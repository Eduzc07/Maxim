#!/usr/bin/env python3
import sys, os, json, time, urllib.request

CACHE_FILE = os.path.expanduser("~/.conky/cache/agy_quota.json")
ENV_FILE = os.path.expanduser("~/.conky/cache/agy_env")
SETTINGS_FILE = os.path.expanduser("~/.gemini/antigravity-cli/settings.json")

def get_active_model():
    if os.path.exists(SETTINGS_FILE):
        try:
            with open(SETTINGS_FILE) as f:
                return json.load(f).get("model", "")
        except Exception:
            pass
    return ""

def refresh_if_needed():
    now = time.time()
    if os.path.exists(CACHE_FILE):
        try:
            mtime = os.path.getmtime(CACHE_FILE)
            if now - mtime < 30: # 30s cache
                return
        except Exception:
            pass

    if not os.path.exists(ENV_FILE):
        return

    ls_addr, csrf_token = None, None
    try:
        with open(ENV_FILE) as f:
            for line in f:
                line = line.strip()
                if line.startswith("ANTIGRAVITY_LS_ADDRESS="):
                    ls_addr = line.split("=", 1)[1].strip('"\'')
                elif line.startswith("ANTIGRAVITY_CSRF_TOKEN="):
                    csrf_token = line.split("=", 1)[1].strip('"\'')
    except Exception:
        return

    if not ls_addr or not csrf_token:
        return

    url = f"http://{ls_addr}/exa.language_server_pb.LanguageServerService/RetrieveUserQuotaSummary"
    req = urllib.request.Request(
        url,
        data=b"{}",
        headers={
            "Content-Type": "application/json",
            "x-codeium-csrf-token": csrf_token
        }
    )
    try:
        with urllib.request.urlopen(req, timeout=1.5) as resp:
            data = resp.read()
            tmp = CACHE_FILE + ".tmp"
            with open(tmp, "wb") as f:
                f.write(data)
            os.replace(tmp, CACHE_FILE)
    except Exception:
        pass

def get_stats():
    refresh_if_needed()
    if not os.path.exists(CACHE_FILE):
        return None

    try:
        with open(CACHE_FILE) as f:
            data = json.load(f)
    except Exception:
        return None

    active_model = get_active_model().lower()

    gemini_models_rem = 1.0
    argon_rem = 1.0
    claude_rem = 1.0

    groups = data.get("response", {}).get("groups", [])
    for g in groups:
        dname = g.get("displayName", "")
        buckets = g.get("buckets", [])
        if dname == "Gemini Models":
            for b in buckets:
                if "weekly" in b.get("bucketId", ""):
                    gemini_models_rem = float(b.get("remainingFraction", 1.0))
        elif dname == "Gemini 4 Argon":
            for b in buckets:
                if "weekly" in b.get("bucketId", ""):
                    argon_rem = float(b.get("remainingFraction", 1.0))
        elif dname == "Claude and GPT models":
            for b in buckets:
                if "weekly" in b.get("bucketId", ""):
                    claude_rem = float(b.get("remainingFraction", 1.0))

    # Detect whether Flash (3.8) or Argon (4) is active
    if "argon" in active_model:
        # If active model is Argon, use Argon quota unless Argon is empty and Gemini has usage
        gem_rem = argon_rem if argon_rem < 1.0 else min(gemini_models_rem, argon_rem)
    else:
        # If Flash or other Gemini model, use standard Gemini quota
        gem_rem = gemini_models_rem

    return {
        "gemini_rem": gem_rem,
        "claude_rem": claude_rem
    }

def main():
    arg = sys.argv[1] if len(sys.argv) > 1 else "summary"
    stats = get_stats()
    if not stats:
        print("0%")
        return

    gem_used = max(0, min(100, int(round((1.0 - stats["gemini_rem"]) * 100))))
    claude_used = max(0, min(100, int(round((1.0 - stats["claude_rem"]) * 100))))

    if arg == "gemini_used":
        print(f"{gem_used}%")
    elif arg == "gemini_bar":
        print(gem_used)
    elif arg == "claude_used":
        print(f"{claude_used}%")
    elif arg == "claude_bar":
        print(claude_used)
    else:
        print(f"Gemini: {gem_used}% | Claude: {claude_used}%")

if __name__ == "__main__":
    main()
