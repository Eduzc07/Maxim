function RTW_Sid2UrlHash() {
	this.urlHashMap = new Array();
	/* <Root>/g
 */
	this.urlHashMap["model:29"] = "model.cpp:213&model.h:89";
	/* <Root>/m
 */
	this.urlHashMap["model:30"] = "model.cpp:214&model.h:90";
	/* <Root>/Jxx */
	this.urlHashMap["model:31"] = "model.cpp:210&model.h:91";
	/* <Root>/Jyy */
	this.urlHashMap["model:41"] = "model.cpp:211&model.h:92";
	/* <Root>/Jzz */
	this.urlHashMap["model:42"] = "model.cpp:212&model.h:93";
	/* <Root>/tau_x */
	this.urlHashMap["model:32"] = "model.cpp:215&model.h:94";
	/* <Root>/tau_y */
	this.urlHashMap["model:43"] = "model.cpp:216&model.h:95";
	/* <Root>/tau_z */
	this.urlHashMap["model:44"] = "model.cpp:217&model.h:96";
	/* <Root>/thrust */
	this.urlHashMap["model:33"] = "model.cpp:218&model.h:97";
	/* <Root>/QuadrotorModel
 */
	this.urlHashMap["model:22"] = "model.cpp:208,496,527,531,571,575&model.h:233";
	/* <Root>/state */
	this.urlHashMap["model:21"] = "model.cpp:491&model.h:102";
	/* <S1>/Integrator */
	this.urlHashMap["model:4"] = "model.cpp:219,492,528,572&model.h:63,68,73";
	/* <S1>/MATLAB Function */
	this.urlHashMap["model:1"] = "model.cpp:209&model.h:58,234";
	/* <S1>/Scope */
	this.urlHashMap["model:6"] = "model.h:214";
	/* <S1>/Scope1 */
	this.urlHashMap["model:13"] = "model.h:215";
	/* <S2>:1 */
	this.urlHashMap["model:1:1"] = "model.cpp:221";
	/* <S2>:1:8 */
	this.urlHashMap["model:1:1:8"] = "model.cpp:226";
	/* <S2>:1:9 */
	this.urlHashMap["model:1:1:9"] = "model.cpp:229";
	/* <S2>:1:10 */
	this.urlHashMap["model:1:1:10"] = "model.cpp:232";
	/* <S2>:1:11 */
	this.urlHashMap["model:1:1:11"] = "model.cpp:235";
	/* <S2>:1:12 */
	this.urlHashMap["model:1:1:12"] = "model.cpp:238";
	/* <S2>:1:13 */
	this.urlHashMap["model:1:1:13"] = "model.cpp:241";
	/* <S2>:1:14 */
	this.urlHashMap["model:1:1:14"] = "model.cpp:244";
	/* <S2>:1:15 */
	this.urlHashMap["model:1:1:15"] = "model.cpp:247";
	/* <S2>:1:16 */
	this.urlHashMap["model:1:1:16"] = "model.cpp:250";
	/* <S2>:1:17 */
	this.urlHashMap["model:1:1:17"] = "model.cpp:251";
	/* <S2>:1:18 */
	this.urlHashMap["model:1:1:18"] = "model.cpp:254";
	/* <S2>:1:19 */
	this.urlHashMap["model:1:1:19"] = "model.cpp:257";
	/* <S2>:1:20 */
	this.urlHashMap["model:1:1:20"] = "model.cpp:260";
	/* <S2>:1:21 */
	this.urlHashMap["model:1:1:21"] = "model.cpp:263";
	/* <S2>:1:22 */
	this.urlHashMap["model:1:1:22"] = "model.cpp:266";
	/* <S2>:1:23 */
	this.urlHashMap["model:1:1:23"] = "model.cpp:269";
	/* <S2>:1:24 */
	this.urlHashMap["model:1:1:24"] = "model.cpp:272";
	/* <S2>:1:25 */
	this.urlHashMap["model:1:1:25"] = "model.cpp:275";
	/* <S2>:1:26 */
	this.urlHashMap["model:1:1:26"] = "model.cpp:278";
	/* <S2>:1:27 */
	this.urlHashMap["model:1:1:27"] = "model.cpp:281";
	/* <S2>:1:28 */
	this.urlHashMap["model:1:1:28"] = "model.cpp:284";
	/* <S2>:1:29 */
	this.urlHashMap["model:1:1:29"] = "model.cpp:287";
	this.getUrlHash = function(sid) { return this.urlHashMap[sid];}
}
RTW_Sid2UrlHash.instance = new RTW_Sid2UrlHash();
function RTW_rtwnameSIDMap() {
	this.rtwnameHashMap = new Array();
	this.sidHashMap = new Array();
	this.rtwnameHashMap["<Root>"] = {sid: "model"};
	this.sidHashMap["model"] = {rtwname: "<Root>"};
	this.rtwnameHashMap["<S1>"] = {sid: "model:22"};
	this.sidHashMap["model:22"] = {rtwname: "<S1>"};
	this.rtwnameHashMap["<S2>"] = {sid: "model:1"};
	this.sidHashMap["model:1"] = {rtwname: "<S2>"};
	this.rtwnameHashMap["<Root>/g "] = {sid: "model:29"};
	this.sidHashMap["model:29"] = {rtwname: "<Root>/g "};
	this.rtwnameHashMap["<Root>/m "] = {sid: "model:30"};
	this.sidHashMap["model:30"] = {rtwname: "<Root>/m "};
	this.rtwnameHashMap["<Root>/Jxx"] = {sid: "model:31"};
	this.sidHashMap["model:31"] = {rtwname: "<Root>/Jxx"};
	this.rtwnameHashMap["<Root>/Jyy"] = {sid: "model:41"};
	this.sidHashMap["model:41"] = {rtwname: "<Root>/Jyy"};
	this.rtwnameHashMap["<Root>/Jzz"] = {sid: "model:42"};
	this.sidHashMap["model:42"] = {rtwname: "<Root>/Jzz"};
	this.rtwnameHashMap["<Root>/tau_x"] = {sid: "model:32"};
	this.sidHashMap["model:32"] = {rtwname: "<Root>/tau_x"};
	this.rtwnameHashMap["<Root>/tau_y"] = {sid: "model:43"};
	this.sidHashMap["model:43"] = {rtwname: "<Root>/tau_y"};
	this.rtwnameHashMap["<Root>/tau_z"] = {sid: "model:44"};
	this.sidHashMap["model:44"] = {rtwname: "<Root>/tau_z"};
	this.rtwnameHashMap["<Root>/thrust"] = {sid: "model:33"};
	this.sidHashMap["model:33"] = {rtwname: "<Root>/thrust"};
	this.rtwnameHashMap["<Root>/QuadrotorModel "] = {sid: "model:22"};
	this.sidHashMap["model:22"] = {rtwname: "<Root>/QuadrotorModel "};
	this.rtwnameHashMap["<Root>/state"] = {sid: "model:21"};
	this.sidHashMap["model:21"] = {rtwname: "<Root>/state"};
	this.rtwnameHashMap["<S1>/g"] = {sid: "model:23"};
	this.sidHashMap["model:23"] = {rtwname: "<S1>/g"};
	this.rtwnameHashMap["<S1>/m"] = {sid: "model:24"};
	this.sidHashMap["model:24"] = {rtwname: "<S1>/m"};
	this.rtwnameHashMap["<S1>/Jxx"] = {sid: "model:25"};
	this.sidHashMap["model:25"] = {rtwname: "<S1>/Jxx"};
	this.rtwnameHashMap["<S1>/Jyy"] = {sid: "model:39"};
	this.sidHashMap["model:39"] = {rtwname: "<S1>/Jyy"};
	this.rtwnameHashMap["<S1>/Jzz"] = {sid: "model:40"};
	this.sidHashMap["model:40"] = {rtwname: "<S1>/Jzz"};
	this.rtwnameHashMap["<S1>/tau_x"] = {sid: "model:27"};
	this.sidHashMap["model:27"] = {rtwname: "<S1>/tau_x"};
	this.rtwnameHashMap["<S1>/tau_y"] = {sid: "model:36"};
	this.sidHashMap["model:36"] = {rtwname: "<S1>/tau_y"};
	this.rtwnameHashMap["<S1>/tau_z"] = {sid: "model:37"};
	this.sidHashMap["model:37"] = {rtwname: "<S1>/tau_z"};
	this.rtwnameHashMap["<S1>/thrust "] = {sid: "model:38"};
	this.sidHashMap["model:38"] = {rtwname: "<S1>/thrust "};
	this.rtwnameHashMap["<S1>/Demux"] = {sid: "model:5"};
	this.sidHashMap["model:5"] = {rtwname: "<S1>/Demux"};
	this.rtwnameHashMap["<S1>/Integrator"] = {sid: "model:4"};
	this.sidHashMap["model:4"] = {rtwname: "<S1>/Integrator"};
	this.rtwnameHashMap["<S1>/MATLAB Function"] = {sid: "model:1"};
	this.sidHashMap["model:1"] = {rtwname: "<S1>/MATLAB Function"};
	this.rtwnameHashMap["<S1>/Scope"] = {sid: "model:6"};
	this.sidHashMap["model:6"] = {rtwname: "<S1>/Scope"};
	this.rtwnameHashMap["<S1>/Scope1"] = {sid: "model:13"};
	this.sidHashMap["model:13"] = {rtwname: "<S1>/Scope1"};
	this.rtwnameHashMap["<S1>/state"] = {sid: "model:28"};
	this.sidHashMap["model:28"] = {rtwname: "<S1>/state"};
	this.rtwnameHashMap["<S2>:1"] = {sid: "model:1:1"};
	this.sidHashMap["model:1:1"] = {rtwname: "<S2>:1"};
	this.rtwnameHashMap["<S2>:1:8"] = {sid: "model:1:1:8"};
	this.sidHashMap["model:1:1:8"] = {rtwname: "<S2>:1:8"};
	this.rtwnameHashMap["<S2>:1:9"] = {sid: "model:1:1:9"};
	this.sidHashMap["model:1:1:9"] = {rtwname: "<S2>:1:9"};
	this.rtwnameHashMap["<S2>:1:10"] = {sid: "model:1:1:10"};
	this.sidHashMap["model:1:1:10"] = {rtwname: "<S2>:1:10"};
	this.rtwnameHashMap["<S2>:1:11"] = {sid: "model:1:1:11"};
	this.sidHashMap["model:1:1:11"] = {rtwname: "<S2>:1:11"};
	this.rtwnameHashMap["<S2>:1:12"] = {sid: "model:1:1:12"};
	this.sidHashMap["model:1:1:12"] = {rtwname: "<S2>:1:12"};
	this.rtwnameHashMap["<S2>:1:13"] = {sid: "model:1:1:13"};
	this.sidHashMap["model:1:1:13"] = {rtwname: "<S2>:1:13"};
	this.rtwnameHashMap["<S2>:1:14"] = {sid: "model:1:1:14"};
	this.sidHashMap["model:1:1:14"] = {rtwname: "<S2>:1:14"};
	this.rtwnameHashMap["<S2>:1:15"] = {sid: "model:1:1:15"};
	this.sidHashMap["model:1:1:15"] = {rtwname: "<S2>:1:15"};
	this.rtwnameHashMap["<S2>:1:16"] = {sid: "model:1:1:16"};
	this.sidHashMap["model:1:1:16"] = {rtwname: "<S2>:1:16"};
	this.rtwnameHashMap["<S2>:1:17"] = {sid: "model:1:1:17"};
	this.sidHashMap["model:1:1:17"] = {rtwname: "<S2>:1:17"};
	this.rtwnameHashMap["<S2>:1:18"] = {sid: "model:1:1:18"};
	this.sidHashMap["model:1:1:18"] = {rtwname: "<S2>:1:18"};
	this.rtwnameHashMap["<S2>:1:19"] = {sid: "model:1:1:19"};
	this.sidHashMap["model:1:1:19"] = {rtwname: "<S2>:1:19"};
	this.rtwnameHashMap["<S2>:1:20"] = {sid: "model:1:1:20"};
	this.sidHashMap["model:1:1:20"] = {rtwname: "<S2>:1:20"};
	this.rtwnameHashMap["<S2>:1:21"] = {sid: "model:1:1:21"};
	this.sidHashMap["model:1:1:21"] = {rtwname: "<S2>:1:21"};
	this.rtwnameHashMap["<S2>:1:22"] = {sid: "model:1:1:22"};
	this.sidHashMap["model:1:1:22"] = {rtwname: "<S2>:1:22"};
	this.rtwnameHashMap["<S2>:1:23"] = {sid: "model:1:1:23"};
	this.sidHashMap["model:1:1:23"] = {rtwname: "<S2>:1:23"};
	this.rtwnameHashMap["<S2>:1:24"] = {sid: "model:1:1:24"};
	this.sidHashMap["model:1:1:24"] = {rtwname: "<S2>:1:24"};
	this.rtwnameHashMap["<S2>:1:25"] = {sid: "model:1:1:25"};
	this.sidHashMap["model:1:1:25"] = {rtwname: "<S2>:1:25"};
	this.rtwnameHashMap["<S2>:1:26"] = {sid: "model:1:1:26"};
	this.sidHashMap["model:1:1:26"] = {rtwname: "<S2>:1:26"};
	this.rtwnameHashMap["<S2>:1:27"] = {sid: "model:1:1:27"};
	this.sidHashMap["model:1:1:27"] = {rtwname: "<S2>:1:27"};
	this.rtwnameHashMap["<S2>:1:28"] = {sid: "model:1:1:28"};
	this.sidHashMap["model:1:1:28"] = {rtwname: "<S2>:1:28"};
	this.rtwnameHashMap["<S2>:1:29"] = {sid: "model:1:1:29"};
	this.sidHashMap["model:1:1:29"] = {rtwname: "<S2>:1:29"};
	this.getSID = function(rtwname) { return this.rtwnameHashMap[rtwname];}
	this.getRtwname = function(sid) { return this.sidHashMap[sid];}
}
RTW_rtwnameSIDMap.instance = new RTW_rtwnameSIDMap();
