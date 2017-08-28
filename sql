#建数据库，utf-8
CREATE DATABASE IF NOT EXISTS testdbName DEFAULT CHARSET utf8 COLLATE utf8_general_ci;
#建WorkLog表
CREATE TABLE `worklog` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `IP` varchar(32) NOT NULL,
  `Name` varchar(128) DEFAULT NULL,
  `Content` longtext NOT NULL,
  `Result` varchar(16) NOT NULL,
  `Time` datetime NOT NULL,
  PRIMARY KEY (`ID`)
)；
#建NTPClient表
CREATE TABLE `NTPClient` (
`ID`  int NOT NULL AUTO_INCREMENT ,
`IP`  varchar(32) NOT NULL ,
`NTPCounts`  int NOT NULL DEFAULT 0 ,
`NTPSCounts`  int NOT NULL DEFAULT 0 ,
`LastNTPType`  varchar(8) NOT NULL ,
`LastUpdateTime`  datetime NOT NULL ,
PRIMARY KEY (`ID`)
)
;

#建ClientRecord表
CREATE TABLE `ClientRecord` (
`ID`  int NOT NULL AUTO_INCREMENT ,
`IP`  varchar(32) NOT NULL ,
`Type`  varchar(16) NOT NULL ,
`Time`  datetime NOT NULL ,
PRIMARY KEY (`ID`)
)
;

#建ClientRecord表中的inset trigger
CREATE TRIGGER `ClientRecord_trigger_insert` AFTER INSERT ON `ClientRecord`
FOR EACH ROW BEGIN
 declare lastIP varchar(32);
 declare countIP int(32);
 declare syncType varchar(16);
 declare lastUpdatetime datetime;
 select IP, Type,Time into lastIP,syncType,lastUpdatetime  from ClientRecord where ID = (select max(ID) from ClientRecord) ;
 set countIP=(select count(*) from NTPClient where IP = lastIP );
 if countIP = 0 then
  if syncType = 'NTP' then
    insert into NTPClient (IP,NTPcounts,LastNTPType,LastUpdateTime) values(lastIP,1,syncType,lastUpdatetime );
    insert into WorkLog(IP,Content,Result,Time)values(lastIP,'client synchronize time from server by NTP','success',lastUpdatetime);
  else
   insert into NTPClient (IP,NTPScounts,LastNTPType,LastUpdateTime) values(lastIP,1,syncType,lastUpdatetime);
   insert into WorkLog(IP,Content,Result,Time)values(lastIP,'client synchronize time from server by NTPS','success',lastUpdatetime);
  end if;
 else
 if syncType = 'NTP' then
  update NTPClient set NTPcounts=NTPcounts + 1 , LastNTPType=syncType,LastUpdateTime = lastUpdatetime where IP=lastIP;
  insert into WorkLog(IP,Content,Result,Time)values(lastIP,'client synchronize time from server by NTP','success',lastUpdatetime);
 else
  update NTPClient set NTPScounts=NTPScounts + 1 , LastNTPType=syncType,LastUpdateTime = lastUpdatetime where IP=lastIP;
  insert into WorkLog(IP,Content,Result,Time)values(lastIP,'client synchronize time from server by NTPS','success',lastUpdatetime);
 end if;
end if;
END;


