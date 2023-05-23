// modernOpenGL.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

//#include <iostream>
#include <crtdbg.h>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <Assertion.hpp>
#include <Application.hpp>
#include <ResourcesManager.hpp>
#include <Model.hpp>

int main()
{
	//Detect memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(822); // Debugger should stop alloc nb

	Log::OpenFile("source/include/Core/Debug/DebugLog.txt");
	ResourcesManager::CreateResource<Model>(std::string("viking_room.obj"));

	//In App ?
	Application app(800, 600);
	app.Update();


	std::cout << "Program ended!\n";

	ResourcesManager::Destroy();
	Log::DeleteInstance();
	return 0;
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
