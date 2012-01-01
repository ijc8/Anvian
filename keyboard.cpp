#include <GL/glut.h>
#include "keyboard.h"

namespace Keyboard {
    bool keys[277]; // 256 ASCII + 21 Special

    void init() {
        for (int i = 0; i < 277; i++)
            keys[i] = false;
        glutKeyboardFunc(keyDown);
        glutKeyboardUpFunc(keyUp);
        glutSpecialFunc(specialKeyDown);
        glutSpecialUpFunc(specialKeyUp);
    }

    void keyDown(unsigned char key, int x, int y) {
        keys[key] = true;
    }

    void keyUp(unsigned char key, int x, int y) {
        keys[key] = false;
    }

    void specialKeyDown(int key, int x, int y) {
        specialKeySwitch(key, true);
    }

    void specialKeyUp(int key, int x, int y) {
        specialKeySwitch(key, false);
    }

    void specialKeySwitch(int key, bool down) {
        // This is ugly, but the GLUT specification doesn't
        // say that the values will be consecutive...
        switch (key) {
            case GLUT_KEY_F1:
                keys[F1] = down; break;
            case GLUT_KEY_F2:
                keys[F2] = down; break;
            case GLUT_KEY_F3:
                keys[F3] = down; break;
            case GLUT_KEY_F4:
                keys[F4] = down; break;
            case GLUT_KEY_F5:
                keys[F5] = down; break;
            case GLUT_KEY_F6:
                keys[F6] = down; break;
            case GLUT_KEY_F7:
                keys[F7] = down; break;
            case GLUT_KEY_F8:
                keys[F8] = down; break;
            case GLUT_KEY_F9:
                keys[F9] = down; break;
            case GLUT_KEY_F10:
                keys[F10] = down; break;
            case GLUT_KEY_F11:
                keys[F11] = down; break;
            case GLUT_KEY_F12:
                keys[F12] = down; break;
            case GLUT_KEY_LEFT:
                keys[LEFT] = down; break;
            case GLUT_KEY_RIGHT:
                keys[RIGHT] = down; break;
            case GLUT_KEY_UP:
                keys[UP] = down; break;
            case GLUT_KEY_DOWN:
                keys[DOWN] = down; break;
            case GLUT_KEY_PAGE_UP:
                keys[PAGEUP] = down; break;
            case GLUT_KEY_PAGE_DOWN:
                keys[PAGEDOWN] = down; break;
            case GLUT_KEY_HOME:
                keys[HOME] = down; break;
            case GLUT_KEY_END:
                keys[END] = down; break;
            case GLUT_KEY_INSERT:
                keys[INSERT] = down; break;
        }
    }
}
