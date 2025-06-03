#include <game.h>

int main() {
    Game myGame(800, 600, "GLCRAFT");

    while (myGame.is_running()) {
        myGame.handle_input();
        myGame.update();
    }
    myGame.exit();
}
