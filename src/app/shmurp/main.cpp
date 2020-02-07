#include "configuration.h"
#include "Game.h"

#include <engine/Application.h>
#include <engine/Timer.h>


using namespace ad;

int main(int argc, const char * argv[])
{
    try
    {
        Application application("Shmurp", conf::gWindowWidth, conf::gWindowHeight,
                                Application::Window_Keep_Ratio);
        Timer timer{glfwGetTime(), 0.};

        shmurp::Game game(application);

        while(application.nextFrame())
        {
            timer.mark(glfwGetTime());
            game.update(timer);
        }
    }
    catch(const std::exception & e)
    {
        std::cerr << "Exception:\n"
                  << e.what()
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::exit(EXIT_SUCCESS);
}

