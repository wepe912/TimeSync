#建数据库，utf-8
CREATE DATABASE IF NOT EXISTS testdbName DEFAULT CHARSET utf8 COLLATE utf8_general_ci;
#建WorkLog表
CREATE TABLE `Worklog` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `IP` varchar(32) NOT NULL,
  `Name` varchar(128) DEFAULT NULL,
  `Content` longtext NOT NULL,
  `Result` varchar(16) NOT NULL,
  `Time` datetime NOT NULL,
  PRIMARY KEY (`ID`)
);
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
#建CountsTable表

CREATE TABLE `CountsTable` (
`TableName`  varchar(32) NOT NULL ,
`Counts`  int NULL DEFAULT 0 ,
PRIMARY KEY (`TableName`)
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
update CountsTable set Counts=Counts+1 where TableName = 'ClientRecordCounts';
END;

#建 TimeStampRecord表
CREATE TABLE `TimeStampRecord` (
`ID`  int NOT NULL AUTO_INCREMENT,
`SerialNum`  varchar(40) NOT NULL ,
`IP`  varchar(32) NOT NULL ,
`Name`  varchar(128) NULL ,
`HashOid`  varchar(128) NOT NULL ,
`HashData`  longblob NOT NULL ,
`TimeStampContent`  longblob NOT NULL ,
`Time`  datetime NOT NULL ,
PRIMARY KEY (`ID`)
)
;
#建TimeStampClient表
CREATE TABLE `TimeStampClient` (
`ID`  int NOT NULL AUTO_INCREMENT,
`IP`  varchar(32) NOT NULL ,
`Name`  varchar(128) NOT NULL ,
`TimeStampSignCounts`  int NOT NULL DEFAULT 0 ,
`LastSignTime`  datetime NULL ,
`TimeStampVerifyCounts`  int NOT NULL DEFAULT 0 ,
`LastVerifyTime`  datetime NULL ,
`Allowed`  int NOT NULL DEFAULT 0 ,
PRIMARY KEY (`ID`)
)
;

#建TimeStampRecord表中的inset trigger
CREATE TRIGGER `TimeStampRecord_trigger_insert` AFTER INSERT ON `TimeStampRecord`
FOR EACH ROW BEGIN
 declare lastIP varchar(32);
 declare countIP int(32);
 declare appName varchar(128);
 declare lastUpdatetime datetime;
 select IP, Name,Time into lastIP,appName,lastUpdatetime  from TimeStampRecord where ID = (select max(ID) from TimeStampRecord) ;
 update TimeStampClient set TimeStampSignCounts=TimeStampSignCounts + 1 , LastSignTime = lastUpdatetime where IP=lastIP and Name=appName;
 insert into WorkLog(IP,Name,Content,Result,Time)values(lastIP,appName,'Signed New TimeStamp ','success',lastUpdatetime);
 update CountsTable set Counts=Counts+1 where TableName = 'TimeStampRecordCounts';
END;




#建Tsync_Black_White_List表
CREATE TABLE `Tsync_Black_White_List` (
`ID`  int NOT NULL AUTO_INCREMENT ,
`IP`  varchar(32) NOT NULL ,
`AllowedNTP`  int NOT NULL DEFAULT 0 ,
`AllowedNTPS`  int NOT NULL DEFAULT 0 ,
`LastChangeTime`  datetime NOT NULL ,
PRIMARY KEY (`ID`)
)
;

建ClientRecord_trigger_delete
CREATE TRIGGER `ClientRecord_trigger_delete` AFTER DELETE ON `clientrecord`
FOR EACH ROW BEGIN
 update CountsTable set Counts=Counts-1 where TableName = 'ClientRecordCounts';
END;;
建TimeStampRecord_trigger_delete
CREATE TRIGGER `TimeStampRecord_trigger_delete` AFTER DELETE ON `TimeStampRecord`
FOR EACH ROW BEGIN
 update CountsTable set Counts=Counts-1 where TableName = 'TimeStampRecordCounts';
END;;






#sql trigger
CREATE TRIGGER insert_test
         AFTER INSERT
            ON ClientRecord
          WHEN (
    SELECT IP
      FROM NTPClient
     WHERE IP = new.IP
     LIMIT 1
)
NOTNULL
BEGIN
    UPDATE CountsTable
       SET Counts = Counts + 1
     WHERE TableName = 'ClientRecordCounts';
END;
#sqlite ClientRecord trigger
CREATE TRIGGER ClientRecord_trigger_insert
         AFTER INSERT
            ON ClientRecord
BEGIN
    UPDATE NTPClient SET NTPCounts = NTPCounts + 1,LastNTPType = 'NTP',LastUpdateTime = new.Time WHERE (SELECT IP FROM NTPClient WHERE IP = new.IP AND new.Type = 'NTP') AND IP = new.IP;
    UPDATE NTPClient SET NTPSCounts = NTPSCounts + 1,LastNTPType = 'NTPS',LastUpdateTime = new.Time WHERE (SELECT IP FROM NTPClient WHERE IP = new.IP AND new.Type = 'NTPS') AND IP = new.IP;
    INSERT INTO NTPClient (IP,LastNTPType,LastUpdateTime) SELECT IP,Type,Time FROM ClientRecord WHERE (SELECT IP FROM NTPClient WHERE IP = new.IP) ISNULL AND IP = new.IP;
    UPDATE NTPClient SET NTPCounts = 1 WHERE (SELECT NTPCounts FROM NTPClient WHERE IP = new.IP)=0 AND ( SELECT NTPSCounts FROM NTPClient  WHERE IP = new.IP )= 0 AND new.Type = 'NTP' AND IP = new.IP;
    UPDATE NTPClient SET NTPSCounts = 1 WHERE ( SELECT NTPCounts FROM NTPClient  WHERE IP = new.IP )= 0 AND ( SELECT NTPSCounts FROM NTPClient WHERE IP = new.IP)=0 AND new.Type = 'NTPS' AND IP = new.IP;
    INSERT INTO Worklog (IP, Content, Result,Time) VALUES (new.IP,new.Type,"success",new.Time);
    update CountsTable set Counts=Counts + 1 where TableName = 'ClientRecordCounts';
END;


#sqlite TimeStampRecord_trigger_insert
CREATE TRIGGER TimeStampRecord_trigger_insert
         AFTER INSERT
            ON TimeStampRecord
      FOR EACH ROW
BEGIN
    UPDATE TimeStampClient
       SET TimeStampSignCounts = TimeStampSignCounts + 1,
           LastSignTime = new.Time
     WHERE IP = new.IP AND 
           Name = new.Name;
    INSERT INTO Worklog (
                            IP,
                            Name,
                            Content,
                            Result,
                            Time
                        )
                        VALUES (
                            new.IP,
                            new.Name,
                            "Sign new TimeStamp",
                            success,
                            new.Time
                        );
    UPDATE CountsTable
       SET Counts = Counts + 1
     WHERE TableName = 'TimeStampRecordCounts';
END;
