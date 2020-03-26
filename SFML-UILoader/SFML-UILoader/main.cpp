#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <fstream>
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <TGUI/TGUI.hpp>
#include <iostream>
#include <chrono>
#include <thread>

std::string userSelectFile()
{
	OPENFILENAME ofn;
	char szFile[260];
	HANDLE hf;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		hf = CreateFile(ofn.lpstrFile,
			GENERIC_READ,
			0,
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);

		CloseHandle(hf);
	}

	

	return std::string(szFile);
}

void TakeScreenshot(sf::RenderWindow& window, const std::string& destinationFile)
{
	sf::Texture outputData;
	outputData.create(window.getSize().x, window.getSize().y);
	outputData.update(window);
	if (outputData.copyToImage().saveToFile(destinationFile))
	{
		std::cout << "Saved image to " << destinationFile << std::endl;
	}
	else
	{
		std::cout << "ERROR: Failed to save screenshot " << destinationFile << std::endl;
	}
}

int main(int argc, char **argv)
{
	std::string sourceFile;
	std::string dstFile;

	if (argc == 3) {
		// first arg is location of source file
		sourceFile = argv[1];

		// second arg is location of destination jpg
		dstFile = argv[2];
	}
	else {
		sourceFile = userSelectFile();
	}

	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Works!");
	tgui::Gui gui(window);
	try {
		gui.loadWidgetsFromFile(sourceFile);
	}
	catch (tgui::Exception exception)
	{
		std::cout << exception.what() << std::endl;
		return 0;
	}

	/*if (!dstFile.empty())
	{
		if (window.isOpen())
		{
			window.clear();
			gui.draw();
			window.display();
			TakeScreenshot(window, dstFile);
		}
		else
		{
			std::cout << "Window is not open, cannot display image" << std::endl;
		}
	}
	else
	{*/
	sf::Clock clock;
	clock.restart();
	sf::Int32 timeElasped = 0;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			gui.handleEvent(event);
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Return)
				{
					TakeScreenshot(window, "output.png");
				}
			}
		}

		window.clear();
		gui.draw();
		window.display();

		if (!dstFile.empty())
		{
			timeElasped += clock.getElapsedTime().asMilliseconds();
			if (timeElasped > 100)
			{
				TakeScreenshot(window, dstFile);
				return 0;
			}
		}
	}
	//}

	gui.removeAllWidgets();
}