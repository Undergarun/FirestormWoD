ALTER TABLE `account`   
  ADD COLUMN `google_auth` VARCHAR(15) NULL AFTER `bnet2_salt`;
  
ALTER TABLE `account`   
  ADD COLUMN `activate` tinyint DEFAULT '0' AFTER `google_auth`;
  
ALTER TABLE `account`   
  ADD COLUMN `rsa_clear` BLOB NULL AFTER `activate`;
  

