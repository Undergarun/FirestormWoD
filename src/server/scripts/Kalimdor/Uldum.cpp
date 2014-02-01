#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "CreatureAI.h"

#define GOSSIP_CHOICE_1  "I'm ready, Doctor Jones !"

enum eHarrisonJonesActions
{
    ACTION_FIRST_MOVE        = 1,
    ACTION_FIRST_JUMP        = 2,
    ACTION_SECOND_JUMP       = 3,
    ACTION_THIRD_JUMP        = 4,
    ACTION_SAY_01            = 5,
    ACTION_SAY_02            = 6
};

enum eHarrisonJonesTexts
{
    SAY_01           = 0,
    SAY_02           = 1,
    SAY_03           = 2
};

#define ACTION_SECOND_MOVE 2
#define ACTION_THIRD_MOVE 3
#define ACTION_FOURTH_MOVE 4
#define ACTION_FIFTH_MOVE 5

class mob_harrison_jones : public CreatureScript
{
	public:
        mob_harrison_jones() : CreatureScript("mob_harrison_jones") { }
		
		bool OnGossipHello(Player* player, Creature* creature)
		{
			if (player->GetQuestStatus(27196) == QUEST_STATUS_INCOMPLETE)
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
				player->SEND_GOSSIP_MENU(16786, creature->GetGUID());
			}

			return true;
		}

		bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
		{
			player->PlayerTalkClass->ClearMenus();

			if (action == GOSSIP_ACTION_INFO_DEF + 1)
			{
				player->CLOSE_GOSSIP_MENU();
				creature->AI()->DoAction(ACTION_FIRST_MOVE);
			}

			return true;
		}

		CreatureAI* GetAI(Creature* creature) const
		{
			return new mob_harrison_jonesAI(creature);
		}

		struct mob_harrison_jonesAI : public ScriptedAI
		{
			mob_harrison_jonesAI(Creature* creature) : ScriptedAI(creature) { }

			bool hasSaid1;
			bool hasSaid2;

			void Reset()
			{
				hasSaid1 = false;
				hasSaid2 = false;
			}

			void DoAction(const int32 action)
			{
				switch (action)
				{
					case ACTION_FIRST_MOVE:
                        Talk(SAY_01);
						me->GetMotionMaster()->MovePoint(0, -9153.513f, -1534.094f, 71.019f);
						break;
					case ACTION_FIRST_JUMP:
						me->GetMotionMaster()->MoveJump(-9148.565f, -1536.0601f, 73.8644f, 2.0f, 2.0f);
						break;
					case ACTION_SECOND_JUMP:
						me->GetMotionMaster()->MoveJump(-9145.193f, -1538.517f, 75.033f, 2.0f, 2.0f);
						break;
					case ACTION_THIRD_JUMP:
						break;
					case ACTION_SAY_02:
						if (hasSaid1 == false)
							Talk(SAY_02);
						break;
					default:
						break;
				}
			}

			void UpdateAI(const uint32 diff)
			{

				std::list<Player*> playerList;
				GetPlayerListInGrid(playerList, me, 15.0f);

				if (me->GetPositionX() == -9153.513f && me->GetPositionY() == -1534.094f && me->GetPositionZ() == 71.019f)
				{
					DoAction(ACTION_SAY_01);
					DoAction(ACTION_FIRST_JUMP);
					hasSaid1 = true;
				}

				if (me->GetPositionX() == -9148.565f && me->GetPositionY() == -1536.0601f && me->GetPositionZ() == 73.8644f)
					DoAction(ACTION_SECOND_JUMP);

				if (me->GetPositionX() == -9145.193f && me->GetPositionY() == -1538.517f && me->GetPositionZ() == 75.033f)
				{
					DoAction(ACTION_SAY_02);
					DoAction(ACTION_THIRD_JUMP);
					hasSaid2 = true;

					for (auto player: playerList)
						player->KilledMonsterCredit(45238);
				}
			}
		};
};

#define NPC_ULDUM_WATCHER 45316

class mob_harrison_jones_2 : public CreatureScript
{
	public:
		mob_harrison_jones_2() : CreatureScript("mob_harrison_jones_2") { }

		bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
		{
			if (quest->GetQuestId() == 27431)
				creature->AI()->DoAction(ACTION_FIRST_MOVE);

			return true;
		}

		CreatureAI* GetAI(Creature* creature) const
		{
			return new mob_harrison_jones_2AI(creature);
		}

		struct mob_harrison_jones_2AI : public ScriptedAI
		{
			mob_harrison_jones_2AI(Creature* creature) : ScriptedAI(creature) { }

			void DoAction(const int32 action)
			{
				switch (action)
				{
					case ACTION_FIRST_MOVE:
						me->GetMotionMaster()->MovePoint(0, -9177.980f, -1532.866f, -172.559f);
						break;
					case ACTION_SECOND_MOVE:
						me->GetMotionMaster()->MovePoint(0, -9170.007f, -1497.185f, -172.558f);
						break;
					case ACTION_THIRD_MOVE:
						me->GetMotionMaster()->MovePoint(0, -9169.322f, -1475.245f, -172.559f);
						break;
					case ACTION_FOURTH_MOVE:
						me->GetMotionMaster()->MovePoint(0, -9161.517f, -1473.894f, -170.909f);
						break;
					case ACTION_FIFTH_MOVE:
                    {
                        std::list<Player*> playerList;
                        GetPlayerListInGrid(playerList, me, 30.0f);

						me->GetMotionMaster()->MovePoint(0, -9145.245f, -1470.735f, -170.909f);
                        for (auto player: playerList)
                            if (player->GetQuestStatus(27431) == QUEST_STATUS_INCOMPLETE)
                                player->KilledMonsterCredit(45742);
						break;
                    }
                    default:
                        break;
				}
			}

			void UpdateAI(const uint32 diff)
			{
				std::list<Creature*> uldumWatcherList;
				me->GetCreatureListWithEntryInGrid(uldumWatcherList, NPC_ULDUM_WATCHER, 100.0f);

				if (me->GetPositionX() <= -9176.0f && me->GetPositionY() <= -1531.0f && me->GetPositionZ() == -172.559f
					&& me->GetPositionX() >= -9178.0f && me->GetPositionY() >= -1533.0f)
					me->AI()->DoAction(ACTION_SECOND_MOVE);

				if (me->GetPositionX() == -9170.007f && me->GetPositionY() == -1497.185f && me->GetPositionZ() == -172.558f)
					me->AI()->DoAction(ACTION_THIRD_MOVE);

				if (me->GetPositionX() == -9169.322f && me->GetPositionY() == -1475.245f && me->GetPositionZ() == -172.559f)
					me->AI()->DoAction(ACTION_FOURTH_MOVE);

				if (me->GetPositionX() == -9161.517f && me->GetPositionY() == -1473.894f && me->GetPositionZ() == -170.909f)
					me->AI()->DoAction(ACTION_FIFTH_MOVE);

				if (me->GetPositionX() == -9145.245f && me->GetPositionY() == -1470.735f && me->GetPositionZ() == -170.909f)
					for (auto uldumWatcher: uldumWatcherList)
						me->EnterVehicle(uldumWatcher, 0);

                // TODO : Camera Systems
			}
		};
};

void AddSC_uldum()
{
    new mob_harrison_jones();
    new mob_harrison_jones_2();
}