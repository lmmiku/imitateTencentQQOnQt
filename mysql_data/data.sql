/*
SQLyog Community v13.1.6 (64 bit)
MySQL - 8.0.33-0ubuntu0.20.04.4 : Database - user_data
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`user_data` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;

USE `user_data`;

/*Table structure for table `chat_history` */

DROP TABLE IF EXISTS `chat_history`;

CREATE TABLE `chat_history` (
  `id` bigint NOT NULL AUTO_INCREMENT,
  `sender_account` varchar(30) DEFAULT NULL,
  `receiver_account` varchar(30) DEFAULT NULL,
  `message` varchar(200) DEFAULT NULL,
  `timestamp` datetime DEFAULT NULL,
  `status` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=81 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

/*Data for the table `chat_history` */

insert  into `chat_history`(`id`,`sender_account`,`receiver_account`,`message`,`timestamp`,`status`) values 
(1,'1','2','Hello, world!','2023-08-15 19:39:32','离线'),
(2,'2','1','saafsdf','2023-08-16 00:25:27','离线'),
(3,'1','2','file_message','2023-08-16 10:37:06','离线'),
(4,'1','2','file_message','2023-08-16 10:39:02','在线'),
(5,'1','2','file_message','2023-08-16 10:39:51','离线'),
(6,'1','2','file_message','2023-08-16 10:39:51','在线'),
(7,'1','2','file_message','2023-08-16 10:40:05','离线');

/*Table structure for table `client_info` */

DROP TABLE IF EXISTS `client_info`;

CREATE TABLE `client_info` (
  `account` varchar(30) DEFAULT NULL,
  `tcp_ip` varchar(40) DEFAULT NULL,
  `tcp_port` int DEFAULT NULL,
  `tcp_socket` int DEFAULT NULL,
  `udp_ip` varchar(40) DEFAULT NULL,
  `udp_port` int DEFAULT NULL,
  `status` varchar(5) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

/*Data for the table `client_info` */

insert  into `client_info`(`account`,`tcp_ip`,`tcp_port`,`tcp_socket`,`udp_ip`,`udp_port`,`status`) values 
('1','0.0.0.0',-1,-1,'192.168.228.1',8000,'离线'),
('2','0.0.0.0',-1,-1,'192.168.228.1',9999,'离线'),
('{2',NULL,NULL,NULL,'192.168.228.1',9999,NULL);

/*Table structure for table `data` */

DROP TABLE IF EXISTS `data`;

CREATE TABLE `data` (
  `account` varchar(30) DEFAULT NULL,
  `username` varchar(60) DEFAULT NULL,
  `password` varchar(60) DEFAULT NULL,
  `head` varchar(150) DEFAULT NULL,
  `signature` varchar(100) DEFAULT NULL,
  `state` varchar(10) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

/*Data for the table `data` */

insert  into `data`(`account`,`username`,`password`,`head`,`signature`,`state`) values 
('1','1','1','C:/Users/shocker/QT practise/qq/res/head/head_7.png',NULL,'在线'),
('17398672253','柠静','1','C:/Users/shocker/QT practise/qq/res/head/head_9.png',NULL,'离线');

/*Table structure for table `friend` */

DROP TABLE IF EXISTS `friend`;

CREATE TABLE `friend` (
  `account` varchar(30) DEFAULT NULL,
  `friend_account` varchar(30) DEFAULT NULL,
  `friend_username` varchar(60) DEFAULT NULL,
  `type` varchar(10) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

/*Data for the table `friend` */

insert  into `friend`(`account`,`friend_account`,`friend_username`,`type`) values 
('17398672253','1','1','好友');

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
