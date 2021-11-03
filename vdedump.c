#define _GNU_SOURCE
#include <arpa/inet.h>
#include <assert.h>
#include <libvdeplug.h>
#include <net/ethernet.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage:\n\npid=\"$(pgrep -n ...)\" %s /path/to/vdedir\n",
            argv[0]);
    return 1;
  }
  if (getenv("pid")) {
    assert(setns(syscall(SYS_pidfd_open, atoi(getenv("pid")), 0),
                 CLONE_NEWNS) >= 0);
  }
  int sock = vde_datafd(vde_open(argv[1], argv[0], 0));
  assert(sock >= 0);
  struct {
    uint32_t magic;
    uint16_t version_major;
    uint16_t version_minor;
    int32_t thiszone;
    uint32_t sigfigs;
    uint32_t snaplen;
    uint32_t linktype;
  } header = {0xa1b2c3d4, 2, 4, 0, 0, 0xffff, 1};
  write(1, &header, sizeof header);
  for (;;) {
    char buf[0xffff];
    ssize_t len = recv(sock, buf, sizeof buf, 0);
    struct timespec tp;
    clock_gettime(CLOCK_REALTIME, &tp);
    struct {
      uint32_t tv_sec;
      uint32_t tv_usec;
      uint32_t caplen;
      uint32_t len;
    } pkthdr = {tp.tv_sec, tp.tv_nsec / 1000, len, len};
    write(1, &pkthdr, sizeof pkthdr);
    write(1, buf, len);
  }
}
