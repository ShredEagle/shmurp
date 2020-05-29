#include "configuration.h"
#include "Game.h"

#include <aunteater/Timer.h>

#include <engine/Application.h>


using namespace ad;

int main(int argc, const char * argv[])
{
    try
    {
        Application application("Shmurp", conf::gWindowWidth, conf::gWindowHeight,
                                Application::Window_Keep_Ratio);
        aunteater::Timer timer{glfwGetTime()};

        shmurp::Game game(application);

        while(application.handleEvents())
        {
            timer.mark(glfwGetTime());
            if(game.update(timer))
            {
                application.swapBuffers();
            }
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

