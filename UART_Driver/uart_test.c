#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#define UART_IOCTL_RESET _IO('u', 0)

int main() {
	int fd = open("/dev/uart_driver", O_RDWR);
	if (fd < 0) {
		perror("open");
		return 1;
	}
	if (ioctl(fd, UART_IOCTL_RESET) == -1) {
		perror("ioctl");
		return 1;
	}
	printf("UART ioctl reset called.\n");
	return 0;
}
