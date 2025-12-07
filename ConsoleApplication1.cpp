#include <iostream>
#include <string_view>
#include <vector>
#include <string>
#include "Random.h"
#include <limits>
#include <cstddef>

namespace Setting
{
	std::vector<std::string_view> words{ "pho", "pizza", "eggroll", "sandwich", "spaghetti", "hamburger", "noddles", "tiramisu", "cookies" , "sausage", "surry", "sushi", "bbq"};

	std::string_view getRandomWord()
	{
		return words[Random::get<std::size_t>(0, words.size() - 1)];
	}

	int numberOfGuess{ 6 };
}

void ignoreLine()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool clearFailedExtraction()
{
	if (!std::cin)
	{
		if (std::cin.eof())
		{
			std::exit(0);
		}

		std::cin.clear();
		return true;
	}
	return false;
}

bool hasUnextractedInput()
{
	return((!std::cin.eof()) && (std::cin.peek() != '\n'));
}

class Session
{

private:
	std::string_view m_word{ Setting::getRandomWord() };
	std::vector<bool> m_guessedLetter{std::vector<bool>(26)};
	int m_numberOfGuess{ Setting::numberOfGuess };
	
public:
	Session() = default;
	std::string_view getWord() const { return m_word; }
	void addGuessedWord(const char& c) { m_guessedLetter[(c % 32) - 1] = true; }
	bool getGuessedLetter(const char& c) const { return m_guessedLetter[(c % 32) - 1]; }
	int getNumberOfGuess() const { return m_numberOfGuess; }
	void decreaseGuess() { --m_numberOfGuess; }
	bool isLetterInWord(char c)
	{
		for (char i : m_word)
		{
			if (i == c)
				return true;
		}

		return false;
	}
	bool winCondition()
	{
		for (const auto& c : m_word)
		{
			if (!getGuessedLetter(c))
				return false;
		}

		return true;
	}
	
};

void printWord( const Session& s)
{
	std::cout << "\nThe word: ";
	for (const auto& i : s.getWord())
	{
		if (s.getGuessedLetter(i))
			std::cout << i << " ";
		else
			std::cout << '_' << ' ';
	}

	std::cout << "\t Guesses left: ";
	for (int i{}; i < s.getNumberOfGuess(); i++)
		std::cout << "+ ";
}

char getUserInput( Session& s )
{
	char c{};
	while (true)
	{
		std::cout << '\n';
		std::cout << "Enter your guess: ";
		std::cin >> c;
		if (clearFailedExtraction() || hasUnextractedInput() || c < 'a' || c > 'z')
		{
			ignoreLine();
			std::cout << "Invalid input. Please try again! \n";
			continue;
		}
		if (s.getGuessedLetter(c))
		{
			std::cout << "Already guessed this letter. Please try again! ";
			ignoreLine();
			continue;
		}
		s.addGuessedWord(c);
		break;
	}
	for (char i : s.getWord())
	{
		if (i == c)
			return c;
	}
	s.decreaseGuess();
	return c;
}

int main()
{
	char userInput{};
	std::cout << "Welcome to food guesser.\n";
	std::cout << "To win: guess the right word. To lose: run out of chances. \n";
	Session s{};
	while (true)
	{
		printWord(s);
		getUserInput(s);

		if (s.getNumberOfGuess() > 0 && s.winCondition())
		{
			std::cout << "\nYou Win!!!\tThe word is " << s.getWord() << '\n';
			while(true)
			{
				std::cout << "\nPress 'y' to play again or 'n' to exit!!!";
				char x{};
				std::cin >> x;
				if (clearFailedExtraction() || hasUnextractedInput() || (x != 'y' && x != 'n'))
				{
					ignoreLine();
					std::cout << "Invalid input. Please try again! \n";
					continue;
				}
				if (x == 'y')
				{
					s = Session{};
					break;
				}
				if (x == 'n')
					std::cout << "Thank you for playing!\n\n";
				return 0;
			}
		}

		if (s.getNumberOfGuess() == 0)
		{
			std::cout << "\nYou Lose!!!\tThe word is " << s.getWord() << '\n';
			while(true)
			{
				std::cout << "\nPress 'y' to play again or 'n' to exit!!!\n";
				char x{};
				std::cin >> x;
				if (clearFailedExtraction() || hasUnextractedInput() || (x != 'y' && x != 'n'))
				{
					ignoreLine();
					std::cout << "Invalid input. Please try again! \n";
					continue;
				}
				if (x == 'y')
				{
					s = Session{};
					break;
				}
				if (x == 'n')
					std::cout << "\nThank you for playing!!!\n\n";
				return 0;
			}
		}
	}


	return 0;
}