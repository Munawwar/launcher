# Launcher

Cross-platform binary launcher

Usage: `launch [binary name]`

Lets say you have a list of binaries like the following:
```
goaws-linux-x86_64
goaws-darwin-arm64
goaws-darwin-x86_64
goaws-windows-x86_64.exe
```

And lets say you want a single command to pick and execute the correct binary on a developer's machine based on the OS they use.

Then you can ship `launch` binary and run

```sh
launch goaws
```
(`goaws` is just an example here, could be any prefix)

And that will run the correct `goaws` binary from the current working directory.

Following OS and architecture filename suffixes are supported:
```
<name>-linux-x86_64
<name>-linux-arm64
<name>-darwin-x86_64
<name>-darwin-arm64
<name>-windows-x86_64.exe
<name>-windows-arm64.exe
<name>-bsd-x86_64
<name>-bsd-arm64
```

## Build the binary yourself?

Download cosmo compiler
```sh
mkdir -p cosmocc
cd cosmocc
wget https://cosmo.zip/pub/cosmocc/cosmocc.zip
unzip cosmocc.zip
cd ..
```

Next, build the file:
```sh
cosmocc/bin/cosmocc -o launch launch.c
```
