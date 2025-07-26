#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <linux/uinput.h>
#include <sys/time.h>
#include <sstream>

int uinput_fd = -1;

void setup_uinput() {
    uinput_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (uinput_fd < 0) {
        perror("open /dev/uinput");
        exit(1);
    }

    ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
    ioctl(uinput_fd, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(uinput_fd, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(uinput_fd, UI_SET_EVBIT, EV_SYN);

    struct uinput_user_dev uidev {};
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "pvpgod_mouse");
    write(uinput_fd, &uidev, sizeof(uidev));
    ioctl(uinput_fd, UI_DEV_CREATE);
    sleep(1);
}

void send_click(int code, int value) {
    input_event ev {};
    gettimeofday(&ev.time, nullptr);
    ev.type = EV_KEY;
    ev.code = code;
    ev.value = value;
    write(uinput_fd, &ev, sizeof(ev));

    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;
    write(uinput_fd, &ev, sizeof(ev));
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: sudo ./pvpgod_mouse /dev/input/eventX\n";
        return 1;
    }

    setup_uinput();

    std::string cmd = "evtest " + std::string(argv[1]);
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        perror("popen evtest");
        return 1;
    }

    char buffer[512];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        std::string line(buffer);
        if (line.find("BTN_LEFT") != std::string::npos ||
            line.find("BTN_RIGHT") != std::string::npos) {

            std::istringstream iss(line);
        std::string tmp;
        int code = 0, value = 0;

        while (iss >> tmp) {
            if (tmp == "code") {
                iss >> code;
            }
            if (tmp == "value") {
                iss >> value;
                break;
            }
        }

        if (code == 272) send_click(BTN_LEFT, value);  // Left
        else if (code == 273) send_click(BTN_RIGHT, value); // Right
            }
    }

    pclose(pipe);
    ioctl(uinput_fd, UI_DEV_DESTROY);
    close(uinput_fd);
    return 0;
}
