namespace Keyboard {
    extern bool keys[277];

    void init();
    void keyUp(unsigned char key, int x, int y);
    void keyDown(unsigned char key, int x, int y);
    void specialKeyUp(int key, int x, int y);
    void specialKeyDown(int key, int x, int y);
    void specialKeySwitch(int key, bool down);

    enum {
        F1 = 256, F2, F3, F4, F5, F6, F7, F8,
        F9, F10, F11, F12, LEFT, RIGHT, UP, DOWN,
        PAGEUP, PAGEDOWN, HOME, END, INSERT
    };
}
