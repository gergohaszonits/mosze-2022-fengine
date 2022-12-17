#ifndef FENGINE_GAME_H_
#define FENGINE_GAME_H_

#include <iostream>
#include "objects/frame.h"
#include "objects/player.h"
#include "filehandler.h"
#include "console.h"
#include "menu.h"
#include "helper.h"

class Game
{
private:
    Player current_player_;
    Frame current_frame_;
    std::vector<Frame> all_frames_;
    std::vector<Player> all_players_;
    bool exit_game_ = false;

public:
    void LoadPlayers()
    {
        all_players_ = ReadPlayers();
    }

    void LoadFrames()
    {
        all_frames_ = ReadFrames();
    }

    void LoadMenu()
    {
        Menu menu(all_frames_, all_players_);
        menu.LoadMainMenu();
        current_player_ = menu.GetCurrentPlayer();
        all_players_ = menu.GetAllPlayer();
    }

    void Start()
    {
        current_frame_ = GetFrameById(current_player_.GetFrameId());
        while (!exit_game_)
        {
            PrintFrame();
            std::string input = Console.ReadLine();
            if (input == "exit")
            {
                return;
            }
            else if (input == "save")
            {
                current_player_.SetFrameId(current_frame_.GetId());
                SavePlayer(current_player_);
                Console.WriteLine("Sikeresen mentve!",PURPLE);
                continue;
            }
             else if (input == "load")
            {
                current_frame_ = GetFrameById(current_player_.GetFrameId());
                continue;
            }
            else if (input == "reset")
            {
                current_frame_ = GetFrameById(0);
                continue;
            }
            else if (input == "wat")
            {
                // system("start https://cdn.eteknix.com/wp-content/uploads/2017/12/1-31.jpg");
                continue;
            }
            else if (StartsWith(input, "go"))
            {
                auto parts = Split(input, " ");
                int id = std::stoi(parts[1]);
                current_frame_ = GetFrameById(id);
                continue;
            }
            if(is_number(input))
            {
                GoToFrame(std::stoi(input));
            }
        }
    }

    void PrintFrame()
    {
        Console.Clear();
        // Animáció ideiglenesen törlésre kerülhet tesztelés idejéig
        Console.WriteLine(current_frame_.GetText(),GREEN);
        uint16_t selector = 1;
        for (auto answers : current_frame_.GetAnswers())
        {
            Console.WriteLine(std::to_string(selector) + ": " + answers.GetText(),BLUE);
            selector++;
        }
    }

    bool GoToFrame(uint16_t selector)
    {
        auto ans = current_frame_.GetAnswers();
        if (selector > ans.size() || selector < 1)
        {
            return false;
        }

        current_frame_ = GetFrameById(ans[selector - 1].GetId());
        return true;
    }

    Frame GetFrameById(uint16_t id_to_get)
    {
        for (auto frame : all_frames_)
        {
            if (id_to_get == frame.GetId())
            {
                return frame;
            }
        }
        return Frame();
    }
    bool is_number(const std::string &s)
    {
        std::string::const_iterator it = s.begin();
        while (it != s.end() && std::isdigit(*it))
            ++it;
        return !s.empty() && it == s.end();
    }
};

#endif