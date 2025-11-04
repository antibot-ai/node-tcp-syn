{
  "targets": [{
    "target_name": "tcp-syn",
    "sources": ["src/tcp-syn.c"],
    "cflags": [
      "-std=c17",
      "-Wall",
      "-Wextra",
      "-Wpedantic",
      "-Wformat",
      "-Wconversion",
      "-Wunused",
      "-O2"
    ]
  }]
}
