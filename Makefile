play: game.cpp
	g++ main.cpp game.cpp -I include -o game -L/opt/homebrew/Cellar/sfml/2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system