/**
 * ...
 * @author Niccolo Abate
 */
class MyButton
{
	var Name:String;
	
	//EveryState can have a textfield
	var Events_MC:MovieClip;
	var Background_MC:MovieClip;
	var RollOver_MC:MovieClip;
	var Pressed_MC:MovieClip;
	
	var Textfield:TextField;
	
	var OnRollOver:Function;
	var OnRollOut:Function;
	var OnPress:Function;
	var OnRelease:Function;
	
	private var bPressed:Boolean = false;
	private var bOver:Boolean = false;
	
	private var Parent:MovieClip;
	private var X:Number;
	private var Y:Number;
	
	static private var AnyButtonPressed:Boolean = false;
	
	private function MouseHitTest(MC:MovieClip):Boolean
	{
		return MC.hitTest(_root._xmouse, _root._ymouse)
	}
	
	public function MyButton(name:String, x:Number, y:Number, parent:MovieClip)
	{
		Name = name;
		X = x;
		Y = y;
		if (parent)
			Parent = parent;
		else
			Parent = _root;
		
		Events_MC = _root.createEmptyMovieClip(Name + "_Events", _root.getNextHighestDepth());
		
		InitializeCallbacks();
	}
	public function Initialize_FileNames(Background_File:String, RollOver_File:String, Pressed_File:String):Void
	{
		Pressed_MC = Parent.attachMovie(Pressed_File, Name + "_Pressed", Parent.getNextHighestDepth());
		Pressed_MC._x = X;
		Pressed_MC._y = Y;
		RollOver_MC = Parent.attachMovie(RollOver_File, Name + "_RollOver", Parent.getNextHighestDepth());
		RollOver_MC._x = X;
		RollOver_MC._y = Y;
		Background_MC = Parent.attachMovie(Background_File, Name + "_Background", Parent.getNextHighestDepth());
		Background_MC._x = X;
		Background_MC._y = Y;
	}
	public function Initialize_Rects(Background_Info:Object, RollOver_Info:Object, Pressed_Info:Object):Void
	{
		if (Pressed_Info)
		{
			if (Pressed_Info.Name == "" || Pressed_Info.Name == null)
				Pressed_Info.Name = Name + "_Pressed";
			if (!Pressed_Info.Parent)
				Pressed_Info.Parent = Parent;
			Pressed_MC = DrawUtils.DrawRect_FromInfo(Pressed_Info);
		}
		if (RollOver_Info)
		{
			if (RollOver_Info.Name == "" || RollOver_Info.Name == null)
				RollOver_Info.Name = Name + "_RollOver";
			if (!RollOver_Info.Parent)
				RollOver_Info.Parent = Parent;
			RollOver_MC = DrawUtils.DrawRect_FromInfo(RollOver_Info);
		}
		if (Background_Info)
		{
			if (Background_Info.Name == "" || Background_Info.Name == null)
				Background_Info.Name = Name + "_Background";
			if (!Background_Info.Parent)
				Background_Info.Parent = Parent;
			Background_MC = DrawUtils.DrawRect_FromInfo(Background_Info);
		}
	}
	public function MakeRectInfo(NameMod:String, W:Number, H:Number, Col:Number, Thickness:Number, BorderCol:Number):Object
	{
		return DrawUtils.MakeRectInfo(Name + NameMod, X, Y, W, H, Col, Thickness, BorderCol, Parent);
	}
	
	private function InitializeCallbacks():Void
	{
		var THIS:MyButton = this;
		Events_MC.onMouseDown = function():Void
		{
			if (THIS.MouseHitTest(THIS.Background_MC))
			{
				THIS.Press();
			}
		}
		Events_MC.onMouseUp = function():Void
		{
			if (THIS.bPressed)
				THIS.Release();
		}
		Events_MC.onMouseMove = function():Void
		{
			if (MyButton.AnyButtonPressed)
				return;
			if (THIS.bOver)
			{
				if (!THIS.MouseHitTest(THIS.Background_MC))
				{
					THIS.RollOut();
				}
			}
			else
			{
				if (THIS.MouseHitTest(THIS.Background_MC))
				{
					THIS.RollOver();
				}
			}
		}
	}
	
	public function Destroy():Void
	{
		Events_MC.removeMovieClip();
		Background_MC.removeMovieClip();
		if (RollOver_MC)
			RollOver_MC.removeMovieClip();
		if (Pressed_MC)
			Pressed_MC.removeMovieClip();
		if (Textfield)
			Textfield.removeTextField();
		delete this;
	}
	
	public function GiveText(Text:String, Col:Number, Size:Number):Void
	{
		if (!Col)
			Col = 0x000000;
		if (!Size)
			Size = 12;
		Textfield = Parent.createTextField(Name + "_Textfield", Parent.getNextHighestDepth(), X, Y, Background_MC._width, Background_MC._height);
		Textfield.multiline = true;
		Textfield.wordWrap = true;
		Textfield.embedFonts = true;
		//Textfield.type = "input";
		var Textformat:TextFormat = new TextFormat();
		Textformat.font = "fonts.arial.ttf";
		Textformat.size = Size;
		Textformat.color = Col;
		Textformat.underline = false;
		Textformat.italic = false;
		Textformat.align = "left";
		Textformat.bold = false;
		Textfield.text = Text;
		Textfield.setTextFormat(Textformat);
	}
	
	public function RollOver():Void
	{
		bOver = true;
		if (RollOver_MC)
		{
			Background_MC.swapDepths(RollOver_MC);
		}
		OnRollOver();
	}
	public function RollOut():Void
	{
		if (RollOver_MC)
		{
			RollOver_MC.swapDepths(Background_MC);
		}
		bOver = false;
		OnRollOut();
	}
	public function Press():Void
	{
		AnyButtonPressed = true;
		bPressed = true;
		if (Pressed_MC)
		{
			//if (RollOver_MC)
			//{
				//RollOver_MC.swapDepths(Background_MC);
			//}
			RollOver_MC.swapDepths(Pressed_MC);
		}
		OnPress();
	}
	public function Release():Void
	{
		AnyButtonPressed = false;
		bPressed = false;
		if (Pressed_MC)
		{
			//if (RollOver_MC)
			//{
			//	RollOver_MC.swapDepths(Background_MC);
			//}
			Pressed_MC.swapDepths(RollOver_MC);
		}
		OnRelease();
	}
	
}