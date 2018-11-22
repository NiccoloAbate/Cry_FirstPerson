/**
 * ...
 * @author Niccolo Abate
 */
class StatBar
{
	public var Index:Number;
	
	private var Bar:MovieClip;
	private var Mask:MovieClip;
	private var Background:MovieClip;
	public var StatName:String;
	public var Stat:Number = 100;
	public var StatMin:Number = 0;
	public var StatMax:Number = 100;
	
	public function StatBar() {}
	public function Destroy()
	{
		Mask.removeMovieClip();
		Bar.removeMovieClip();
		Background.removeMovieClip();
		delete this;
	}
	
	public function Initialize(Name:String, BarType:String):Void
	{
		var Bar_Background_File:String = StatBarInfo.GetBarBacrgroundFileFromType(BarType);
		if (Bar_Background_File)
		{
			Background = _root.attachMovie(Bar_Background_File, Name + "_Background", _root.getNextHighestDepth());
		}
		var Bar_File:String = StatBarInfo.GetBarFileFromType(BarType);
		Bar = _root.attachMovie(Bar_File, Name + "_Bar", _root.getNextHighestDepth());
		Mask = DrawUtils.DrawRect(0, 0, Bar._width, Bar._height, 0x000000, Name + "_Mask");
		Bar.setMask(Mask);
	}
	public function SetPos(X:Number, Y:Number):Void
	{
		X -= (Bar._width / 2);
		Y -= StatBarInfo.Y_OFFSET;
		if (Background)
		{
			Background._x = X;
			Background._y = Y;
		}
		Bar._x = X;
		Bar._y = Y;
		Mask._x = X;
		Mask._y = Y;
	}
	public function SetScale(Scale:Number):Void
	{
		Background._xscale = Scale * 100;
		Background._yscale = Scale * 100;
		Bar._xscale = Scale * 100;
		Bar._yscale = Scale * 100;
		Mask._xscale = Scale * 100;
		Mask._yscale = Scale * 100;
	}
	public function InitializeStat(Name:String, Min:Number, Max:Number, Val:Number):Void
	{
		StatName = Name;
		StatMin = Min;
		StatMax = Max;
		Stat = Val;
	}
	public function UpdateMask():Void
	{
		Mask._width = (Bar._width * ((Stat - StatMin) / (StatMax - StatMin)));
	}
	public function SetVisible(Visible:Boolean):Void
	{
		Bar._visible = Visible;
		Background._visible = Visible;
	}
}