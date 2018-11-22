/**
 * ...
 * @author Niccolo Abate
 */
class Main 
{
	
	var StatBars:Array = new Array;
	
	static var MAIN:Main;
	public static function main(swfRoot:MovieClip):Void 
	{
		Main.MAIN = new Main();
		
		/*
		var Index:Number = _root.NewStatBar("Test", "Health", 200, 200, "Health", 0, 100, 50);
		_root.SetStatBarPos(Index, 600, 200);
		_root.SetStatBarValue(Index, 25);
		_root.SetStatBarMax(Index, 50);
		_root.SetStatBarMin(Index, 25);
		_root.SetStatBarStat(Index, "Health", 0, 1000, 500);
		_root.DeleteStatBar(Index);
		*/
	}
	
	public function Main() 
	{	
		_root.NewStatBar = function(Name:String, Type:String, X:Number, Y:Number, StatName:String, StatMin:Number, StatMax:Number, Stat:Number):Number
		{
			return Main.MAIN.NewStatBar(Name, Type, X, Y, StatName, StatMin, StatMax, Stat);
		}
		_root.SetStatBarPos = function(Index:Number, X:Number, Y:Number):Void
		{
			Main.MAIN.SetStatBarPos(Index, X, Y);
		}
		_root.SetStatBarScale = function(Index:Number, Scale:Number):Void
		{
			Main.MAIN.SetStatBarScale(Index, Scale);
		}
		_root.SetStatBarValue = function(Index:Number, Stat:Number):Void
		{
			Main.MAIN.SetStatBarValue(Index, Stat);
		}
		_root.SetStatBarMax = function(Index:Number, Max:Number):Void
		{
			Main.MAIN.SetStatBarMax(Index, Max);
		}
		_root.SetStatBarMin = function(Index:Number, Min:Number):Void
		{
			Main.MAIN.SetStatBarMin(Index, Min);
		}
		_root.SetStatBarStat = function(Index:Number, Name:String, Min:Number, Max:Number, Stat:Number):Void
		{
			Main.MAIN.SetStatBarStat(Index, Name, Min, Max, Stat);
		}
		_root.DeleteStatBar = function(Index:Number):Void
		{
			Main.MAIN.DeleteStatBar(Index);
		}
		_root.SetStatBarVisible = function(Index:Number, Visible:Boolean):Void
		{
			Main.MAIN.SetStatBarVisible(Index, Visible);
		}
	}
	
	public function NewStatBar(Name:String, Type:String, X:Number, Y:Number, StatName:String, StatMin:Number, StatMax:Number, Stat:Number):Number
	{
		var Bar:StatBar = new StatBar();
		Bar.Initialize(Name, Type);
		Bar.SetPos(X, Y);
		Bar.InitializeStat(StatName, StatMin, StatMax, Stat);
		Bar.UpdateMask();
		Bar.Index = StatBars.push(Bar) - 1;
		return Bar.Index;
	}
	public function SetStatBarPos(Index:Number, X:Number, Y:Number):Void
	{
		var Bar:StatBar = StatBars[Index];
		Bar.SetPos(X, Y);
		Bar.UpdateMask();
	}
	public function SetStatBarScale(Index:Number, Scale:Number):Void
	{
		var Bar:StatBar = StatBars[Index];
		Bar.SetScale(Scale);
	}
	public function SetStatBarValue(Index:Number, Stat:Number):Void
	{
		var Bar:StatBar = StatBars[Index];
		Bar.Stat = Stat;
		Bar.UpdateMask();
	}
	public function SetStatBarMax(Index:Number, Max:Number):Void
	{
		var Bar:StatBar = StatBars[Index];
		Bar.StatMax = Max;
		Bar.UpdateMask();
	}
	public function SetStatBarMin(Index:Number, Min:Number):Void
	{
		var Bar:StatBar = StatBars[Index];
		Bar.StatMin = Min;
		Bar.UpdateMask();
	}
	public function SetStatBarStat(Index:Number, Name:String, Min:Number, Max:Number, Stat:Number):Void
	{
		var Bar:StatBar = StatBars[Index];
		Bar.InitializeStat(Name, Min, Max, Stat);
		Bar.UpdateMask();
	}
	public function DeleteStatBar(Index:Number):Void
	{
		var Bar:StatBar = StatBars[Index];
		Bar.Destroy();
		StatBars[Index] = null;
	}
	public function SetStatBarVisible(Index:Number, Visible:Boolean):Void
	{
		var Bar:StatBar = StatBars[Index];
		Bar.SetVisible(Visible);
	}
	
}