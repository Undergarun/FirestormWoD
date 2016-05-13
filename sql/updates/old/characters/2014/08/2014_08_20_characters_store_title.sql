ALTER TABLE .`store_title`   
  CHANGE `transaction` `transaction` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
  ADD KEY(`transaction`);
  
CREATE TABLE `log_store_title` 
(
	`id` int (10),
	`transaction` int (10),
	`guid` int (10),
	`title` int (10),
	`date` timestamp 
); 