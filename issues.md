[container loses time when the machine is sleeping]
- https://github.com/docker/for-win/issues/72
- on 20191229, the date in the container was 210191226 (plus time difference)
- `hwclock` didn't work, couldn't access the rtc device
- trying to `apk add ntpdate` failed
- don't want to run a privleged powershell just to restart docker from the command line
- just have to restart docker manually...
