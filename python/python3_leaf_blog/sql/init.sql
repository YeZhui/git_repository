--create database yeBlogDb;
--grant all on yeBlogDb.* to mysql@localhost identified by 'mysql';

use yeBlogDb;

drop table if exists `users`;
create table `users` (
    `id` varchar(50) not null,
    `email` varchar(50) not null,
    `passwd` varchar(50) not null,
    `admin` bool not null,
    `name` varchar(50) not null,
    `image` varchar(500) not null,
    `created_at` real not null,
    unique key `idx_email` (`email`),
    key `idx_created_at` (`created_at`),
    primary key (`id`)
) engine=innodb default charset=utf8;
-- 插入一条管理员账号 password:admin123
insert into users(`id`,`email`,`passwd`,`admin`,`name`,`image`,`created_at`)
values('0', 'guanyezhui@163.com', '6d894877e248f50b4f357cc53bb59781dc0518df',1, '晟夏的叶', 'http://www.gravatar.com/avatar/64e1b8d34f425d19e1ee2ea7236d3028?d=mm&s=120', 1491478052.34864);

insert into users(`id`,`email`,`passwd`,`admin`,`name`,`image`,`created_at`)
values('1', '1363200680@qq.com', '6d894877e248f50b4f357cc53bb59781dc0518df',1, '晟夏', 'http://www.gravatar.com/avatar/64e1b8d34f425d19e1ee2ea7236d3028?d=mm&s=120', 1491478052.34864);

drop table if exists blogs;
create table blogs (
    `id` varchar(50) not null,
    `user_id` varchar(50) not null,
    `user_name` varchar(50) not null,
    `user_image` varchar(500) not null,
	`classes_id` varchar(50),
	`classes_name` varchar(50),
    `name` varchar(50) not null,
    `summary` varchar(200) not null,
    `content` mediumtext not null,
    `created_at` real not null,
	`blog_date` varchar(10) not null,
    key `idx_created_at` (`created_at`),
    primary key (`id`)
) engine=innodb default charset=utf8;

drop table if exists comments;
create table comments (
    `id` varchar(50) not null,
    `blog_id` varchar(50) not null,
    `user_id` varchar(50) not null,
    `user_name` varchar(50) not null,
    `user_image` varchar(500) not null,
    `content` mediumtext not null,
    `created_at` real not null,
    key `idx_created_at` (`created_at`),
    primary key (`id`)
) engine=innodb default charset=utf8;

drop table if exists classes;
create table classes (
	`id` varchar(50) not null,
	`name` varchar(50) not null,
	`created_at` real not null,
	key `idx_created_at` (`created_at`),
	primary key (`id`)
) engine=innodb default charset=utf8;

drop table if exists drafts;
create table drafts (
    `id` varchar(50) not null,
    `user_id` varchar(50) not null,
    `user_name` varchar(50) not null,
    `user_image` varchar(500) not null,
	`classes_id` varchar(50),
    `name` varchar(50) not null,
    `summary` varchar(200) not null,
    `content` mediumtext not null,
    `created_at` real not null,
    key `idx_created_at` (`created_at`),
    primary key (`id`)
) engine=innodb default charset=utf8;