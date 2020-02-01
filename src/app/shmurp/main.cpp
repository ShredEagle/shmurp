#include <engine/Application.h>
#include <engine/Timer.h>


using namespace ad;

int main(int argc, const char * argv[])
{
    try
    {
        Application application("Shmurp", 600, 800);
        ad::Timer timer{glfwGetTime(), 0.};

        while(application.nextFrame())
        {
            timer.mark(glfwGetTime());
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

