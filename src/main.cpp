#include "game.h"

LeStateManager* LeStateManager::m_instance = 0;

int main( int argc, char* args[] )
{
LeGame game;
	game.start();    
	//game.stop();
	
        
return 0;
}

#ifdef OS_WINDOWS
int WinMain( int argc, char* argv[] )
{
	//windows complains for undefinded symbol , this is stupid workaround
	main(argc,argv);
}
#endif
