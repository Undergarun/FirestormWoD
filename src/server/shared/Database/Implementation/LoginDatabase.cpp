////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "LoginDatabase.h"

void LoginDatabaseConnection::DoPrepareStatements()
{
    if (!m_reconnecting)
        m_stmts.resize(MAX_LOGINDATABASE_STATEMENTS);

    PREPARE_STATEMENT(LOGIN_SEL_REALMLIST, "SELECT id, name, address, port, icon, flag, timezone, allowedSecurityLevel, population, gamebuild FROM realmlist WHERE flag <> 3 ORDER BY name", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_DEL_EXPIRED_IP_BANS, "DELETE FROM ip_banned WHERE unbandate<>bandate AND unbandate<=UNIX_TIMESTAMP()", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_UPD_EXPIRED_ACCOUNT_BANS, "UPDATE account_banned SET active = 0 WHERE active = 1 AND unbandate<>bandate AND unbandate<=UNIX_TIMESTAMP()", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_SEL_IP_BANNED, "SELECT * FROM ip_banned WHERE ip = ?", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_INS_IP_AUTO_BANNED, "INSERT INTO ip_banned VALUES (?, UNIX_TIMESTAMP(), UNIX_TIMESTAMP()+?, 'Trinity realmd', 'Failed login autoban')", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_SEL_IP_BANNED_ALL, "SELECT ip, bandate, unbandate, bannedby, banreason FROM ip_banned WHERE (bandate = unbandate OR unbandate > UNIX_TIMESTAMP()) ORDER BY unbandate", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_IP_BANNED_BY_IP, "SELECT ip, bandate, unbandate, bannedby, banreason FROM ip_banned WHERE (bandate = unbandate OR unbandate > UNIX_TIMESTAMP()) AND ip LIKE CONCAT('%%', ?, '%%') ORDER BY unbandate", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_ACCOUNT_BANNED, "SELECT bandate, unbandate FROM account_banned WHERE id = ? AND active = 1", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_SEL_ACCOUNT_BANNED_ALL, "SELECT account.id, username FROM account, account_banned WHERE account.id = account_banned.id AND active = 1 GROUP BY account.id", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_ACCOUNT_BANNED_BY_USERNAME, "SELECT account.id, username FROM account, account_banned WHERE account.id = account_banned.id AND active = 1 AND username LIKE CONCAT('%%', ?, '%%') GROUP BY account.id", CONNECTION_SYNCH);
    //PREPARE_STATEMENT(LOGIN_INS_ACCOUNT_AUTO_BANNED, "INSERT INTO account_banned VALUES (?, UNIX_TIMESTAMP(), UNIX_TIMESTAMP()+?, 'Trinity realmd', 'Failed login autoban', 1)", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_DEL_ACCOUNT_BANNED, "DELETE FROM account_banned WHERE id = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_SEL_SESSIONKEY, "SELECT a.sessionkey, a.id, aa.gmlevel  FROM account a LEFT JOIN account_access aa ON (a.id = aa.id) WHERE username = ?", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_UPD_VS, "UPDATE account SET v = ?, s = ? WHERE username = ?", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_UPD_LOGONPROOF, "UPDATE account SET sessionkey = ?, last_ip = ?, last_login = NOW(), locale = ?, failed_logins = 0, os = ? WHERE username = ?", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_SEL_LOGONCHALLENGE, "SELECT a.sha_pass_hash, a.id, a.locked, a.last_ip, aa.gmlevel, a.v, a.s, a.token_key, a.bnet2_pass_hash, a.bnet2_salt FROM account a LEFT JOIN account_access aa ON (a.id = aa.id) WHERE a.username = ?", CONNECTION_SYNCH)
    //PREPARE_STATEMENT(LOGIN_INS_LOG_IP, "INSERT IGNORE INTO account_log_ip (`accountid`, `ip`, `date`) VALUES (?, ?, NOW())", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_UPD_FAILEDLOGINS, "UPDATE account SET failed_logins = failed_logins + 1 WHERE username = ?", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_SEL_FAILEDLOGINS, "SELECT id, failed_logins FROM account WHERE username = ?", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_SEL_ACCOUNT_ID_BY_NAME, "SELECT id FROM account WHERE username = ?", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_SEL_ACCOUNT_LIST_BY_NAME, "SELECT id, username FROM account WHERE username = ?", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_ACCOUNT_INFO_BY_NAME, "SELECT id, sessionkey, last_ip, locked, v, s, expansion, mutetime, locale, recruiter, os FROM account WHERE username = ?", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_ACCOUNT_LIST_BY_EMAIL, "SELECT id, username FROM account WHERE email = ?", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_NUM_CHARS_ON_REALM, "SELECT numchars FROM realmcharacters WHERE realmid = ? AND acctid= ?", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_SEL_ACCOUNT_BY_IP, "SELECT id, username FROM account WHERE last_ip = ?", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_SEL_ACCOUNT_BY_ID, "SELECT 1 FROM account WHERE id = ?", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_INS_IP_BANNED, "INSERT INTO ip_banned VALUES (?, UNIX_TIMESTAMP(), UNIX_TIMESTAMP()+?, ?, ?)", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_DEL_IP_NOT_BANNED, "DELETE FROM ip_banned WHERE ip = ?", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_INS_ACCOUNT_BANNED, "INSERT INTO account_banned VALUES (?, 0, UNIX_TIMESTAMP(), UNIX_TIMESTAMP()+?, ?, ?, 1)", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_SEL_ACCOUNT_ALWAYS_BANNED, "SELECT unbandate-UNIX_TIMESTAMP() AS unban FROM account_banned WHERE id = ? AND active = 1 AND bandate <> unbandate", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_SEL_ACCOUNT_BANNED_PERMANENT, "SELECT 1 FROM account_banned WHERE id = ? AND active = 1 AND bandate = unbandate", CONNECTION_SYNCH)
    PREPARE_STATEMENT(LOGIN_UPD_ACCOUNT_NOT_BANNED, "UPDATE account_banned SET active = 0 WHERE id = ? AND active != 0", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_DEL_REALM_CHARACTERS_BY_REALM, "DELETE FROM realmcharacters WHERE acctid = ? AND realmid = ?", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_DEL_REALM_CHARACTERS, "DELETE FROM realmcharacters WHERE acctid = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_INS_REALM_CHARACTERS, "INSERT INTO realmcharacters (numchars, acctid, realmid) VALUES (?, ?, ?)", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_UPD_REALM_CHARACTERS, "UPDATE realmcharacters SET numchars = ? WHERE acctid = ? AND realmid = ?", CONNECTION_ASYNC)
    PREPARE_STATEMENT(LOGIN_SEL_SUM_REALM_CHARACTERS, "SELECT SUM(numchars) FROM realmcharacters WHERE acctid = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_INS_ACCOUNT, "INSERT INTO account(username, sha_pass_hash, joindate) VALUES(?, ?, NOW())", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_INS_REALM_CHARACTERS_INIT, "INSERT INTO realmcharacters (realmid, acctid, numchars) SELECT realmlist.id, account.id, 0 FROM realmlist, account LEFT JOIN realmcharacters ON acctid=account.id WHERE acctid IS NULL", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_UPD_EXPANSION, "UPDATE account SET expansion = ? WHERE id = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_UPD_ACCOUNT_LOCK, "UPDATE account SET locked = ? WHERE id = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_INS_LOG, "INSERT INTO logs (time, realm, type, level, string) VALUES (?, ?, ?, ?, ?)", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_UPD_USERNAME, "UPDATE account SET v = 0, s = 0, username = ?, sha_pass_hash = ? WHERE id = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_UPD_PASSWORD, "UPDATE account SET v = 0, s = 0, sha_pass_hash = ? WHERE id = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_UPD_MUTE_TIME, "UPDATE account SET mutetime = ? WHERE id = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_UPD_LAST_IP, "UPDATE account SET last_ip = ? WHERE username = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_UPD_ACCOUNT_ONLINE, "UPDATE account SET online = 1 WHERE id = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_UPD_UPTIME_PLAYERS, "UPDATE uptime SET uptime = ?, maxplayers = ? WHERE realmid = ? AND starttime = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_DEL_OLD_LOGS, "DELETE FROM logs WHERE (time + ?) < ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_DEL_ACCOUNT_ACCESS, "DELETE FROM account_access WHERE id = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_DEL_ACCOUNT_ACCESS_BY_REALM, "DELETE FROM account_access WHERE id = ? AND (RealmID = ? OR RealmID = -1)", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_INS_ACCOUNT_ACCESS, "INSERT INTO account_access (id,gmlevel,RealmID) VALUES (?, ?, ?)", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_GET_ACCOUNT_ID_BY_USERNAME, "SELECT id FROM account WHERE username = ?", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_GET_ACCOUNT_ACCESS_GMLEVEL, "SELECT gmlevel FROM account_access WHERE id = ?", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_GET_GMLEVEL_BY_REALMID, "SELECT gmlevel FROM account_access WHERE id = ? AND (RealmID = ? OR RealmID = -1)", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_GET_USERNAME_BY_ID, "SELECT username FROM account WHERE id = ?", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_CHECK_PASSWORD, "SELECT 1 FROM account WHERE id = ? AND sha_pass_hash = ?", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_CHECK_PASSWORD_BY_NAME, "SELECT 1 FROM account WHERE username = ? AND sha_pass_hash = ?", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_PINFO, "SELECT a.username, aa.gmlevel, a.email, a.last_ip, DATE_FORMAT(a.last_login, '%Y-%m-%d %T'), a.mutetime FROM account a LEFT JOIN account_access aa ON (a.id = aa.id AND (aa.RealmID = ? OR aa.RealmID = -1)) WHERE a.id = ?", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_PINFO_BANS, "SELECT unbandate, bandate = unbandate, bannedby, banreason FROM account_banned WHERE id = ? AND active ORDER BY bandate ASC LIMIT 1", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_GM_ACCOUNTS, "SELECT a.username, aa.gmlevel FROM account a, account_access aa WHERE a.id=aa.id AND aa.gmlevel >= ? AND (aa.realmid = -1 OR aa.realmid = ?)", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_ACCOUNT_INFO, "SELECT a.username, a.last_ip, aa.gmlevel, a.expansion FROM account a LEFT JOIN account_access aa ON (a.id = aa.id) WHERE a.id = ?", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_ACCOUNT_ACCESS_GMLEVEL_TEST, "SELECT 1 FROM account_access WHERE id = ? AND gmlevel > ?", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_ACCOUNT_ACCESS, "SELECT a.id, aa.gmlevel, aa.RealmID FROM account a LEFT JOIN account_access aa ON (a.id = aa.id) WHERE a.username = ?", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_ACCOUNT_RECRUITER, "SELECT 1 FROM account WHERE recruiter = ?", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_BANS, "SELECT 1 FROM account_banned WHERE id = ? AND active = 1 UNION SELECT 1 FROM ip_banned WHERE ip = ?", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_ACCOUNT_WHOIS, "SELECT username, email, last_ip FROM account WHERE id = ?", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_REALMLIST_SECURITY_LEVEL, "SELECT allowedSecurityLevel from realmlist WHERE id = ?", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_DEL_ACCOUNT, "DELETE FROM account WHERE id = ?", CONNECTION_ASYNC);

    PREPARE_STATEMENT(LOGIN_INS_CHAR_SPELL, "INSERT INTO account_spell (accountId, spell, active, disabled, IsMountFavorite, groupRealmMask) VALUES (?, ?, ?, ?, ?, ?) ON DUPLICATE KEY UPDATE groupRealmMask = groupRealmMask | ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_SEL_CHARACTER_SPELL, "SELECT spell, active, disabled, IsMountFavorite, groupRealmMask FROM account_spell WHERE accountId = ? AND spell < 197205", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_DEL_CHAR_SPELL_BY_SPELL, "DELETE FROM account_spell WHERE spell = ? AND accountId = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_DEL_CHAR_SPELL, "DELETE FROM account_spell WHERE accountId = ?", CONNECTION_ASYNC);

    PREPARE_STATEMENT(LOGIN_UPD_ACCOUNT_PREMIUM, "UPDATE account_premium SET active = 0 WHERE active = 1 AND unsetdate<=UNIX_TIMESTAMP() AND unsetdate<>setdate", CONNECTION_ASYNC);

    // Transfers
    PREPARE_STATEMENT(LOGIN_SEL_TRANSFERS_DUMP, "SELECT `id`, `account`, `guid` FROM webshop_delivery_interrealm_transfer WHERE `startrealm` = ? AND state = 0", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_SEL_TRANSFERS_LOAD, "SELECT `id`, `account`, `guid`, `dump` FROM webshop_delivery_interrealm_transfer WHERE `destrealm` = ? AND state = 1", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_UPD_TRANSFER_PDUMP, "UPDATE webshop_delivery_interrealm_transfer SET state = 1, dump = ? WHERE id = ?", CONNECTION_SYNCH);
    PREPARE_STATEMENT(LOGIN_SEL_TRANSFERS_EXP_LOAD, "SELECT `id`, `account`, `guid`, `dump` FROM webshop_delivery_interexp_transfer WHERE `destrealm` = ? AND state = 1", CONNECTION_ASYNC);

    // Battle pets
#define PETBATTLE_FIELDS "slot, name, nameTimeStamp, species, quality, breed, level, xp, display, health, flags, infoPower, infoMaxHealth, infoSpeed, infoGender, account, declinedGenitive, declinedNative, declinedAccusative, declinedInstrumental, declinedPrepositional"
#define PETBATTLE_FULL_FIELDS "id, " PETBATTLE_FIELDS
    PREPARE_STATEMENT(LOGIN_SEL_PETBATTLE_ACCOUNT, "SELECT " PETBATTLE_FULL_FIELDS " FROM account_battlepet WHERE account = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_REP_PETBATTLE, "REPLACE INTO account_battlepet(" PETBATTLE_FULL_FIELDS ") VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_INS_PETBATTLE, "INSERT INTO account_battlepet(" PETBATTLE_FIELDS ") VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)", CONNECTION_BOTH);
#undef PETBATTLE_FIELDS
#undef PETBATTLE_FULL_FIELDS

    /// Battlepay
    PREPARE_STATEMENT(LOGIN_SEL_BATTLEPAY_POINTS, "SELECT points FROM account_battlepay WHERE accountId = ?", CONNECTION_ASYNC);
    
    //////////////////////////////////////////////////////////////////////////
    /// Heirloom Collection
    PREPARE_STATEMENT(LOGIN_SEL_HEIRLOOM_COLLECTION, "SELECT heirloom_id, upgrade_flags, groupRealmMask FROM account_heirlooms WHERE account_id = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_INS_HEIRLOOM, "INSERT INTO account_heirlooms (account_id, heirloom_id, upgrade_flags, groupRealmMask) VALUE (?, ?, ?, ?) ON DUPLICATE KEY UPDATE groupRealmMask = groupRealmMask | ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_UPD_HEILOOM_FLAGS, "UPDATE account_heirlooms SET upgrade_flags = ? WHERE account_id = ? AND heirloom_id = ?", CONNECTION_ASYNC);
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    /// ToyBox
    PREPARE_STATEMENT(LOGIN_SEL_ACCOUNT_TOYS, "SELECT item_id, is_favorite FROM account_toys WHERE account_id = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_INS_ACCOUNT_TOYS, "INSERT INTO account_toys (account_id, item_id, is_favorite) VALUE (?, ?, ?)", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_UPD_TOY_FAVORITE, "UPDATE account_toys SET is_favorite = ? WHERE account_id = ? AND item_id = ?", CONNECTION_ASYNC);
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    /// User Reporting
    PREPARE_STATEMENT(LOGIN_UPD_USER_REPORTING_STEP, "UPDATE user_reporting SET step = ?, last_ip = ? WHERE account_id = ? AND step < ?", CONNECTION_ASYNC);
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    /// Services
    PREPARE_STATEMENT(LOGIN_REMOVE_ACCOUNT_SERVICE, "UPDATE account SET service_flags = service_flags &~ ? WHERE id = ?", CONNECTION_ASYNC);
    PREPARE_STATEMENT(LOGIN_SET_ACCOUNT_SERVICE, "UPDATE account SET service_flags = service_flags | ? WHERE id = ?", CONNECTION_ASYNC);
    //////////////////////////////////////////////////////////////////////////

    PREPARE_STATEMENT(LOGIN_RPL_CHARACTER_RENDERER_QUEUE, "REPLACE INTO character_renderer_queue (guid, race, gender, class, skinColor, face, hairStyle, hairColor, facialHair, equipment) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
}
