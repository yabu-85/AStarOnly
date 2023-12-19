#pragma once
#include <array>
#include <vector>
#include <utility> //std::pair

class PlayerCommand
{
public:
	enum COMMAND {
		ATK = 0,
		SUB_ATK,
		CENTER,
		CENTER_UP,
		CENTER_DOWN,
		WEAPON_SELECT,
		AVO,
		TARGET,
		LEFT,
		RIGHT,
		UP,
		DOWN,
		CMD_MAX
	};

private:
	std::vector<std::pair<int, COMMAND>> pushKeyCommand_;
	std::vector<std::pair<int, COMMAND>> downKeyCommand_;
	std::vector<std::pair<int, COMMAND>> pushMouseCommand_;
	std::vector<std::pair<int, COMMAND>> downMouseCommand_;
	std::array<bool, CMD_MAX> commandFlags = { false };

	bool walk_;

public:
	PlayerCommand();
	void Update();

	//KeyConfigデータをデフォルトの状態に戻す
	void SetDefaultKeyConfig();

	//コマンドの変更
	//number COMMANDの番号（ATK)
	//conf	 Key,Mouseの番号（DIK_A：０）
	//type 0 = p_key : 1 = d_key : 2 = p_mouse : 3 = d_mouse
	void ChangeCmd(COMMAND number, int conf, int type);
	
	//指定したCmdをリスト（pushKeyCommandなど）から削除
	void DeleteCmd(COMMAND number);

	bool CmdAtk() { return commandFlags[ATK]; }
	bool CmdSubAtk() { return commandFlags[SUB_ATK]; }
	bool CmdAvo() { return commandFlags[AVO]; }
	bool CmdTarget() { return commandFlags[TARGET]; }
	bool CmdCenter() { return commandFlags[CENTER]; }
	bool CmdCenterUp() { return commandFlags[CENTER_UP]; }
	bool CmdCenterDown() { return commandFlags[CENTER_DOWN]; }
	bool CmdWeaponSelect() { return commandFlags[WEAPON_SELECT]; }
	bool CmdLeft() { return commandFlags[LEFT]; }
	bool CmdRight() { return commandFlags[RIGHT]; }
	bool CmdUp() { return commandFlags[UP]; }
	bool CmdDown() { return commandFlags[DOWN]; }
	bool CmdWalk() { return walk_; }

};

