# Launcher

Cross-platform binary launcher

Usage: `launch [binary name]`

Lets say you have a list of binaries like the following:
```
goaws-linux-x86_64
goaws-darwin-arm64
goaws-darwin-x86_64
goaws-windows-x86_64
```

And lets say you want a single command to pick and execute the correct binary on a developer's machine based on the OS they use.

Then you can ship `launch` binary and run

```
launch goaws
```
(`goaws` is just an example here, could be any prefix)

And that will run the correct `goaws` binary from the current working directory.

Following OS and architecture filname suffix combinations are supported:
```
<name>-linux-x86_64
<name>-linux-arm64
<name>-linux-armv7
<name>-linux-armv6
<name>-darwin-x86_64
<name>-darwin-arm64
<name>-windows-x86_64.exe
<name>-windows-arm64.exe
<name>-bsd-x86_64
<name>-bsd-arm64
```
