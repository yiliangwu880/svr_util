
唯一id  高32用timestamp,低32用自增。  1秒内，产生少于4亿，就能保证唯一
{
	一般几行代码，重复跑4亿次，都要十几秒了。 所以实际情况不会重复
}
用户层timer
