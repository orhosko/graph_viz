int* get_msg_buffer() {
    return (int*)msg_buffer;
}

void fill_msg_buffer(){
    for (int i = 0; i < 16; i++) {
        msg_buffer[i] = '1';
    }
}

void m_strncpy(char* dest, const char* src, int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
    dest[n] = '\0';
}

char msg2[] = "msg2";

int* get_msg2() {
    return (int*)msg2;
}
