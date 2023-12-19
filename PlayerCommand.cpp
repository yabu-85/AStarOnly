#include "PlayerCommand.h"
#include "Engine/Input.h"

PlayerCommand::PlayerCommand()
	: walk_(false)
{
	SetDefaultKeyConfig();
	
	//ChangeCmd(ATK, DIK_G, 1);
}

void PlayerCommand::Update()
{
	for (const auto& pair : pushKeyCommand_)
	commandFlags[pair.second] = Input::IsKey(pair.first);
	
	for (const auto& pair : downKeyCommand_)
	commandFlags[pair.second] = Input::IsKeyDown(pair.first);
	
	for (const auto& pair : pushMouseCommand_)
	commandFlags[pair.second] = Input::IsMouseButton(pair.first);
	
	for (const auto& pair : downMouseCommand_)
	commandFlags[pair.second] = Input::IsMouseButtonDown(pair.first);
	
	//Scroll : Walk
	commandFlags[CENTER_UP] = Input::IsUpScroll();
	commandFlags[CENTER_DOWN] = Input::IsDownScroll();
	walk_ = commandFlags[LEFT] || commandFlags[RIGHT] || commandFlags[UP] || commandFlags[DOWN];

	return;
}

void PlayerCommand::SetDefaultKeyConfig()
{
	pushKeyCommand_.clear();
	pushKeyCommand_.push_back(std::make_pair(DIK_A, LEFT));
	pushKeyCommand_.push_back(std::make_pair(DIK_D, RIGHT));
	pushKeyCommand_.push_back(std::make_pair(DIK_W, UP));
	pushKeyCommand_.push_back(std::make_pair(DIK_S, DOWN));

	downKeyCommand_.clear();
	downKeyCommand_.push_back(std::make_pair(DIK_SPACE, AVO));
	downKeyCommand_.push_back(std::make_pair(DIK_Q, TARGET));

	pushMouseCommand_.clear();
	pushMouseCommand_.push_back(std::make_pair(0, WEAPON_SELECT));

	downMouseCommand_.clear();
	downMouseCommand_.push_back(std::make_pair(0, ATK));
	downMouseCommand_.push_back(std::make_pair(1, SUB_ATK));
	downMouseCommand_.push_back(std::make_pair(2, CENTER));
}

void PlayerCommand::ChangeCmd(COMMAND number, int conf, int type)
{
	//新しく追加した要素と同じものがある場合は・・・なんかやる？
	
	commandFlags[number] = false;
	DeleteCmd(number);

	if (type == 0) {
		pushKeyCommand_.push_back(std::make_pair(conf, number));
		return;
	}
	if (type == 1) {
		downKeyCommand_.push_back(std::make_pair(conf, number));
		return;
	}
	if (type == 2) {
		pushMouseCommand_.push_back(std::make_pair(conf, number));
		return;
	}
	if (type == 3) {
		downMouseCommand_.push_back(std::make_pair(conf, number));
		return;
	}

}

void PlayerCommand::DeleteCmd(COMMAND number)
{
	for (int i = 0; i < pushKeyCommand_.size(); i++) {
		if (pushKeyCommand_[i].second == number) {
			pushKeyCommand_.erase(pushKeyCommand_.begin() + i);
			return;
		}
	}

	for (int i = 0; i < downKeyCommand_.size(); i++) {
		if (downKeyCommand_[i].second == number) {
			downKeyCommand_.erase(downKeyCommand_.begin() + i);
			return;
		}
	}

	for (int i = 0; i < pushMouseCommand_.size(); i++) {
		if (pushMouseCommand_[i].second == number) {
			pushMouseCommand_.erase(pushMouseCommand_.begin() + i);
			return;
		}
	}

	for (int i = 0; i < downMouseCommand_.size(); i++) {
		if (downMouseCommand_[i].second == number) {
			downMouseCommand_.erase(downMouseCommand_.begin() + i);
			return;
		}
	}
}
