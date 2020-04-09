# Test Jetson Nano

Check Image Size
```
v4l2-ctl --list-formats-ext
```

Start Fan
```
sudo jetson_clocks
```

```
some_command & disown
```

```
systemctl enable --now atd.service
```

```
echo "shotCamera" | at 05:00 AM
```

## copy videos
```
rsync -avz edu@192.168.1.14:/home/edu/Workspace/pythonExamples/videos/07042020 ~/jetson_nano_videos/
```

## copy images
```
rsync -avz edu@192.168.1.14:/home/edu/Workspace/pythonExamples/images/07042020 ~/jetson_nano_images/
```

```
rsync -avz --exclude README.md * edu@192.168.1.14:~/Workspace/pythonExamples
```

```
./timeLapsed.py --hours 0.05 --interval 2
```

| Frames in 1 minute  | Frames in 1 Hour | FPS | Interval (seg) | Finally  |
| :-:|:-: |:-------------:| :-----:|:-----:|
| 5 (each 12 sec)  | 300          | 30   | 12 |10 seg -> 1h |
| 30 (each 2 sec)  | 1800          | 30  | 2 |60 seg -> 1h |
