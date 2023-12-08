// BullCowCartridge.cpp
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay()
{
    Super::BeginPlay();
    Words = GetValidWords(Isograms);
    SetupGame();

    PrintLine(TEXT("The number of possible words is %i."), Words.Num());
    PrintLine(TEXT("The number of valid words is %i"), Words.Num());
}

void UBullCowCartridge::OnInput(const FString& Input)
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Welcome to the game!"));

    RandomPosition = FMath::RandRange(0, Words.Num() - 1);
    HiddenWord = Words[RandomPosition];

    PrintLine(TEXT("The hidden word is %s and is item %i."), *HiddenWord, RandomPosition);

    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Guess a %i letter word and press Enter."), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives remaining."), Lives);
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You won! Good for you."));
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Nope! The hidden word is %i characters long. Try again!"), HiddenWord.Len());
        PrintLine(TEXT("You still have %i lives left."), Lives);
        return;
    }

    // Make isogram check here
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters! Try again!"));
        return;
    }

    --Lives;
    PrintLine(TEXT("You lost a life. \nYou have %i lives remaining."), Lives);

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You are out of lives. \nTry again later."));
        PrintLine(TEXT("The correct answer was: %s"), *HiddenWord);
        EndGame();
        return;
    }

    int32 Bulls, Cows;
    GetBullCows(Guess, Bulls, Cows);
    PrintLine(TEXT("You have %i Bulls and %i Cows."), Bulls, Cows);

    FString BullCowHint;
    for (int32 Index = 0; Index < HiddenWord.Len(); Index++)
    {
        if (Guess[Index] == HiddenWord[Index])
        {
            BullCowHint += "B";
        }
        else if (HiddenWord.Contains(Guess[Index]))
        {
            BullCowHint += "C";
        }
        else
        {
            BullCowHint += "_";
        }
    }

    PrintLine(TEXT("Bull and Cow Hint: %s"), *BullCowHint);
}

bool UBullCowCartridge::IsIsogram(const FString Word) const
{
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

void UBullCowCartridge::GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const
{
    BullCount = 0;
    CowCount = 0;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            BullCount++;
        }
        else if (HiddenWord.Contains(Guess[GuessIndex]))
        {
            CowCount++;
        }
    }
}
