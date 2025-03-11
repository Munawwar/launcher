#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/utsname.h>

const char* detect_os() {
  struct utsname buffer;
  if (uname(&buffer) == 0) {
    if (strstr(buffer.sysname, "Linux") != NULL) {
      return "linux";
    } else if (strstr(buffer.sysname, "Darwin") != NULL) {
      return "darwin";
    } else if (
      strstr(buffer.sysname, "FreeBSD") != NULL ||
      strstr(buffer.sysname, "OpenBSD") != NULL ||
      strstr(buffer.sysname, "NetBSD") != NULL
    ) {
      return "bsd";
    } else if (strstr(buffer.sysname, "Windows") != NULL) {
      return "windows";
    }
  }
  return "unknown";
}

const char* detect_arch() {
  struct utsname buffer;
  if (uname(&buffer) == 0) {
    if (strstr(buffer.machine, "x86_64") != NULL ||
      strstr(buffer.machine, "amd64") != NULL) {
      return "x86_64";
    }
    if (strstr(buffer.machine, "aarch64") != NULL ||
      strstr(buffer.machine, "arm64") != NULL) {
      return "arm64";
    }
    if (strstr(buffer.machine, "armv7") != NULL) {
      return "armv7";
    }
    if (strstr(buffer.machine, "armv6") != NULL) {
      return "armv6";
    }
    // Generic ARM detection
    if (strstr(buffer.machine, "arm") != NULL) {
      // Try to determine ARM version
      FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
      if (cpuinfo) {
        char line[256];
        while (fgets(line, sizeof(line), cpuinfo)) {
          if (strstr(line, "ARMv7") != NULL) {
            fclose(cpuinfo);
            return "armv7";
          } else if (strstr(line, "ARMv6") != NULL) {
            fclose(cpuinfo);
            return "armv6";
          }
        }
        fclose(cpuinfo);
        // Default to ARMv7 for generic ARM
        return "armv7";
      } 
      // If we can't determine the version, assume ARMv7
      return "armv7";
    }
    return "unknown";
  }
  
  // Fallback to pointer size
  if (sizeof(void*) == 8) {
    return "x86_64"; // Assume x86_64 for 64-bit
  } else {
    return "x86"; // Assume x86 for 32-bit
  }
}

int main(int argc, char *argv[]) {
  const char *binary_path = NULL;
  const char *binary_prefix = NULL;
  char path_buffer[256];
  
  // Get binary prefix from command line
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <binary-prefix> [args...]\n", argv[0]);
    fprintf(stderr, "Example: %s goaws-0.5.2\n", argv[0]);
    return 1;
  }
  
  binary_prefix = argv[1];
  
  // Detect OS and architecture safely
  const char *os = detect_os();
  const char *arch = detect_arch();
  
  printf("Detected OS: %s\n", os);
  printf("Detected architecture: %s\n", arch);
  
  // Construct binary path based on detected OS and architecture
  if (strcmp(os, "windows") == 0) {
    snprintf(path_buffer, sizeof(path_buffer), "%s-%s-%s.exe", binary_prefix, os, arch);
  } else {
    snprintf(path_buffer, sizeof(path_buffer), "%s-%s-%s", binary_prefix, os, arch);
  }
  
  binary_path = path_buffer;
  printf("Looking for binary: %s\n", binary_path);
  
  // Check if the binary exists
  if (access(binary_path, X_OK) != 0) {
    fprintf(stderr, "Binary %s not found or not executable\n", binary_path);
    return 1;
  }
  
  printf("Launching: %s\n", binary_path);
  
  // Forward all arguments to the selected binary (skipping our binary name and the prefix argument)
  char *new_argv[argc];
  new_argv[0] = (char *)binary_path;
  for (int i = 2; i < argc; i++) {
    new_argv[i-1] = argv[i];
  }
  new_argv[argc-1] = NULL;
  
  // Execute the selected binary
  execv(binary_path, new_argv);

  // If execv succeeded, we won't reach here. If it does, execv failed
  fprintf(stderr, "Failed to execute %s: %s\n", binary_path, strerror(errno));
  return 1;
}