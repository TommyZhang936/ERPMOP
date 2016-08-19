--//定义视图
IF EXISTS (SELECT * FROM sysobjects WHERE /*检测是否存在*/  
                         name = 'v_orderprice')  
     DROP VIEW v_orderprice /*删除视图*/ 
go
create view v_orderprice
as
select	d60.rkey 
	,d60.sales_order				as order_no
	--,CUSTOMER_PTR
	,d10.cust_code 
	,d10.abbr_name 
	--,CUST_PART_PTR
	,d50.customer_part_number		as prod_no
	,d50.cp_rev1					as prod_ver
	,d60.PARTS_ORDERED				as pcs_qty
	,d50.set_x_qty					as x
	,d50.set_y_qty					as y
	,d60.set_ordered				as set_qty
	,d60.part_price					as pcs_price
	,d60.set_price					as set_price
	,d60.exch_rate					as ex_rate
	,d60.so_style
	,d60.status
	,d50.cp_rev						as prod_verA
	,d60.analysis_code_5			as MP_remark
	,d50.RKEY						as D50rkey
	
from data0060 d60
join data0050 d50 on d60.CUST_PART_PTR	= d50.rkey
join data0010 d10 on d60.CUSTOMER_PTR	= d10.rkey
where so_style = '0' 
AND [STATUS] NOT IN (3, 4, 5)

go
--视图定义结束

--创建用户表----------------------------------------------
if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[myuser]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)

create table myuser
(
id int identity(1,1) primary key, 
name varchar(50) not null,
cname varchar(50) not null,
password varchar(50) not null,
permission varchar(10)
)
go
-----------------------------

insert into myuser values ('admin','管理员','123','A')
insert into myuser values('liww','黎薇薇','abc123','A')

--修改字段
IF not EXISTS (
SELECT name FROM syscolumns
WHERE (name = 'permission') AND (id IN
          (SELECT id FROM sysobjects WHERE name = 'myuser'))
          ) 
begin
alter table muyser  add permission varchar(10) null  --用户权限
end
go