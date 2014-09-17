ALTER TABLE `account`   
  ADD COLUMN `google_auth` VARCHAR(15) NULL AFTER `bnet2_salt`;
  
ALTER TABLE `account`   
  ADD COLUMN `rsa_clear` BLOB NULL AFTER `google_auth`;
