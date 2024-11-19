

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


CREATE TABLE `control_status` (
  `id` int(11) NOT NULL,
  `name` varchar(50) NOT NULL,
  `status` int(11) NOT NULL CHECK (`name` = 'motor' and `status` in (-1,0,1) or `name` = 'servodirection' and `status` in (-1,0,1) or `name` = 'servoaxis' and `status` between -6 and 6 or `name` = 'servolift' and `status` between -3 and 3 or `name` = 'servogrip' and `status` between -5 and 0),
  `timestamp` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;


INSERT INTO `control_status` (`id`, `name`, `status`, `timestamp`) VALUES
(1, 'motor', 1, '2024-09-19 04:00:06'),
(2, 'servodirection', 0, '2024-09-19 03:37:52'),
(3, 'servoaxis', 0, '2024-09-19 03:37:52'),
(4, 'servolift', 0, '2024-09-19 03:37:52'),
(5, 'servogrip', 0, '2024-09-19 03:37:52');


ALTER TABLE `control_status`
  ADD PRIMARY KEY (`id`);


ALTER TABLE `control_status`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;
COMMIT;
