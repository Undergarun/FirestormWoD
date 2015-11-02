#include <ScriptPCH.h>
#include <ScriptMgr.h>

class CharacterRenderer : public PlayerScript
{
    private:
        ACE_Based::LockedMap<uint32, QueryResultFuture> m_QueryResultFutures;

    public:
        CharacterRenderer() : PlayerScript("CharacterRenderer")
        {
        }

        void OnLogout(Player* p_Player) override
        {
            uint8 l_Race       = p_Player->getRace();
            uint8 l_Gender     = p_Player->getGender();
            uint8 l_Class      = p_Player->getClass();
            uint8 l_SkinColor  = p_Player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_SKIN_ID);
            uint8 l_Face       = p_Player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_FACE_ID);
            uint8 l_HairStyle  = p_Player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_HAIR_STYLE_ID);
            uint8 l_Haircolor  = p_Player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_HAIR_COLOR_ID);
            uint8 l_Facialhair = p_Player->GetByteValue(PLAYER_FIELD_REST_STATE, PLAYER_BYTES_2_OFFSET_FACIAL_STYLE);

            
            std::ostringstream l_Equipement;
            l_Equipement << "[";
            
            for (uint8 l_Slot = 0; l_Slot < EQUIPMENT_SLOT_END; l_Slot++)
            {
                if (Item* l_Item = p_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, l_Slot))
                    l_Equipement << "[" << l_Item->GetTemplate()->InventoryType << "," << l_Item->GetTemplate()->DisplayInfoID << "],";
            }

            l_Equipement << "]";

            PreparedStatement* l_Statement = LoginDatabase.GetPreparedStatement(LOGIN_RPL_CHARACTER_RENDERER_QUEUE);
            l_Statement->setUInt32(0, p_Player->GetGUIDLow() | g_RealmID << 24);
            l_Statement->setUInt32(1, l_Race);
            l_Statement->setUInt32(2, l_Gender);
            l_Statement->setUInt32(3, l_Class);
            l_Statement->setUInt32(4, l_SkinColor);
            l_Statement->setUInt32(5, l_Face);
            l_Statement->setUInt32(6, l_HairStyle);
            l_Statement->setUInt32(7, l_Haircolor);
            l_Statement->setUInt32(8, l_Facialhair);
            l_Statement->setString(9, l_Equipement.str().c_str());
            LoginDatabase.Execute(l_Statement);
        }
};

void AddSC_Character_Renderer()
{
    new CharacterRenderer();
};
