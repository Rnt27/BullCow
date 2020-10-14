// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

//#include "Misc/FileHelper.h"
//#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
        
    Valids = GetValidWords(Words);

    //Setting up the game
    SetupGame();

    //PrintLine(TEXT("The number of possible valid words is %i"), Valids.Num());

/* 
    //Calling the list of words possible:
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("C://Users//Owner//Downloads//BullCowGame-starter-kit//BullCowGame-starter-kit//Content//WordLists//HiddenWorldList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);

    FString JoinedStr;

    JoinedStr += TEXT("Tamales");

    //JoinedStr += Words.GetData(0);
    
    for (FString Str: Words){

        JoinedStr += Str;
        JoinedStr += TEXT(" ");

    }

    PrintLine(TEXT("%s"), *JoinedStr); */

    //Debug Line
    //Included in optimization, the FString::Printf... no need to write the below part
    //PrintLine(TEXT("The hidden word is: %s."), *HiddenWord);
    //PrintLine(FString::Printf(TEXT("The hidden word is: %s"), *HiddenWord));
    
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{

    /* If game is over, then ClearScreen and SetupGame() the game
    else checking PlayerGuess */
    if(bGameOver){

        ClearScreen();
        SetupGame();

    }
    else{

        ProcessGuess(Input);

    }
        
}

void UBullCowCartridge::SetupGame(){

    //Welcome player
    PrintLine(TEXT("Welcome to Bull Cows!"));
    
    //Setting the hidden word
    HiddenWord = Valids[FMath::RandRange(0, Valids.Num() - 1)];
    //Set Lives
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word."), HiddenWord.Len());    //Magic number remove!
    PrintLine(TEXT("You have these many lives: %i"), Lives);
    PrintLine(TEXT("Type in your guess and \npress enter to continue."));
    
    //PrintLine(TEXT("The hidden word is: %s."), *HiddenWord);
    
    /* //example of array and such handling
    const TCHAR HW[] = TEXT("cakes");
    //const TCHAR HW[] = {TEXT('c'), TEXT('a'), TEXT('k'), TEXT('e'), TEXT('s'), TEXT('\0')};
    PrintLine(TEXT("Character 1 of the hidden word is: %c"), HiddenWord[0]); */

}

void UBullCowCartridge::EndGame(){

    bGameOver = true;

    PrintLine(TEXT("\nPress enter to play again."));

}

void UBullCowCartridge::ProcessGuess(const FString& Guess){

    //Checking if word guessed is right
    if(Guess == HiddenWord){
            
        PrintLine(TEXT("You have won!"));
        EndGame();
        return;

    }

    //Check if Isogram
    if(!IsIsogram(Guess)){

        PrintLine(TEXT("There shouldn't be any repeating letters! \nGuess again."));
        return;

    }

    //Prompt to Guess Again
    //Check right number of characters
    //Prompt to Guess Again
        
    

    if(HiddenWord.Len() != Guess.Len()){

        PrintLine(TEXT("Sorry, \nthe hidden word is %i characters long. \nTry guessing again."), HiddenWord.Len());
        return;

    }

    //Reduce Lives
    --Lives;

    PrintLine(TEXT("Life lost!"));
        
    if(Lives <= 0){

        ClearScreen();
        PrintLine(TEXT("You have no lives left! \nThe hidden word is %s. \nYou have lost!"), *HiddenWord);
        EndGame();
        return;

    }

    //Show the player Bulls and Cows (Out Parameters, not initialized, but called right away)
    //int32 Bulls, Cows;
    //GetBullCows(Guess, Bulls, Cows);
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

    PrintLine(TEXT("Guess again! \nYou have this many lives remaining: %i"), Lives);

    int32 RandomNumber =  FMath::RandRange(0, HiddenWord.Len() - 1);

    PrintLine(TEXT("Helpful Random Hint: \nLetter at position number %i is: %c."), RandomNumber + 1, HiddenWord[RandomNumber]);

}

//const cuz it doesn't change variables
bool UBullCowCartridge::IsIsogram(const FString& Word) const{

    for(int32 Index = 0; Index < Word.Len(); Index ++){

        for(int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++){

            if(Word[Index] == Word[Comparison]){
            return false;

            }

        }

    }

    /* int32 Index = 0;
    int32 Comparison = Index + 1; */

    /* for (int32 Index = 0, Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
    {
        if(Word[Index] == Word[Comparison]){
            return false;
        }
    } */
    

    /* for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        PrintLine(TEXT("%c"), Word[Index]);
    } */
    

    return true;

}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const{

    TArray<FString> ValidWords;

    for (FString Word : WordList){

        if(Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word)){

            ValidWords.Emplace(Word);

        }

    }
    return ValidWords;

}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const{

    FBullCowCount Count;
    
    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if(Guess[GuessIndex] == HiddenWord[GuessIndex]){

            Count.Bulls++;
            continue;

        }

        for(int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++){

            if(Guess[GuessIndex] == HiddenWord[HiddenIndex] && HiddenIndex != GuessIndex){

                Count.Cows++;
                break;

            }

        }
    }
    
    return Count;

}