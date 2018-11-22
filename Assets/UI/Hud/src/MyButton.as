/**
 * ...
 * @author Niccolo Abate
 */
class MyButton
{
	
	var Name:String;
	var Id:Number = -1;
	var X:Number;
	var Y:Number;
	var W:Number;
	var H:Number;
	private var Background:MovieClip;
	private var Background_RollOver:MovieClip;
	private var Background_Pressed:MovieClip;
	private var Textfield:TextField;
	private var Textformat:TextFormat;
	var TextBorder:Number;
	
	private var Events:MovieClip;
	private var Over:Boolean = false;
	private var Pressed:Boolean = false;
	static private var AnyButtonPressed:Boolean = false;
	
	var OnRollOver:Function = function(){};
	var OnRollOut:Function = function(){};
	var OnPress:Function = function(){};
	var OnRelease:Function = function(){};
	
	public function MyButton(){}
	public function Destroy()
	{
		Background.removeMovieClip();
		Background_RollOver.removeMovieClip();
		Background_Pressed.removeMovieClip();
		Textfield.removeTextField();
		delete Textformat;
		delete this;
	}
	
	private function InitializeMovieClips(BG_File:String, BG_RollOver_File:String, BG_Pressed_File:String):Void
	{
		Background = _root.attachMovie(BG_File, Name + "_BG", _root.getNextHighestDepth());
		Background._x = X;
		Background._y = Y;
		Background_RollOver = _root.attachMovie(BG_RollOver_File, Name + "_BG_RollOver", _root.getNextHighestDepth());
		Background_RollOver._x = X;
		Background_RollOver._y = Y;
		Background_RollOver._visible = false;
		Background_Pressed = _root.attachMovie(BG_Pressed_File, Name + "_BG_Pressed", _root.getNextHighestDepth());
		Background_Pressed._x = X;
		Background_Pressed._y = Y;
		Background_Pressed._visible = false;
	}
	private function InitializeTextfield(Text:String, textformat:TextFormat):Void
	{
		Textformat = textformat;
		Textfield = _root.createTextField(Name + "_Textfield", _root.getNextHighestDepth(), X + TextBorder, Y + TextBorder, W - (TextBorder * 2), H - (TextBorder * 2));
		Textfield.multiline = true;
		Textfield.wordWrap = true;
		Textfield.embedFonts = true;
		Textfield.text = Text;
		Textfield.setTextFormat(Textformat);
	}
	private function InitializeEvents():Void
	{
		Events = _root.createEmptyMovieClip(Name + "_Events", _root.getNextHighestDepth());
		var THIS:MyButton = this;
		Events.onMouseMove = function()
		{
			if (MyButton.AnyButtonPressed && !THIS.Pressed)
				return;
			if (THIS.Over)
			{
				if((_root._xmouse < THIS.X) || (_root._xmouse > THIS.X + THIS.W)
					|| (_root._ymouse < THIS.Y) || (_root._ymouse > THIS.Y + THIS.H))
				{
					THIS.RollOut();
				}
			}
			else
			{
				if((_root._xmouse > THIS.X) && (_root._xmouse < THIS.X + THIS.W)
					&& (_root._ymouse > THIS.Y) && (_root._ymouse < THIS.Y + THIS.H))
				{
					THIS.RollOver();
				}
			}
		}
		Events.onMouseDown = function()
		{
			if (THIS.Over)
				THIS.Press();
		}
		Events.onMouseUp = function()
		{
			if (THIS.Pressed)
				THIS.Release();
		}
	}
	public function Initialize(name:String, x:Number, y:Number, w:Number, h:Number, BG_File:String, BG_RollOver_File:String, BG_Pressed_File:String, Text:String, Textformat:TextFormat, textBorder:Number):Void
	{
		Name = name;
		X = x;
		Y = y;
		W = w;
		H = h;
		TextBorder = textBorder;
		
		InitializeMovieClips(BG_File, BG_RollOver_File, BG_Pressed_File);
		InitializeTextfield(Text, Textformat);
		InitializeEvents();
	}
	
	private function RollOver():Void
	{
		Over = true;
		
		if (Pressed)
			return;
		
		if (Background_RollOver)
		{
			Background._visible = false;
			Background_RollOver._visible = true;
		}
		
		OnRollOver();
	}
	private function RollOut():Void
	{
		Over = false;
		
		if (Pressed)
			return;
		
		if (Background_RollOver)
		{
			Background._visible = true;
			Background_RollOver._visible = false;
		}
		
		OnRollOut();
	}
	private function Press():Void
	{
		AnyButtonPressed = true;
		Pressed = true;
		
		if (Background_Pressed)
		{
			Background_RollOver._visible = false;
			Background_Pressed._visible = true;
		}
		
		OnPress();
	}
	private function Release():Void
	{
		AnyButtonPressed = false;
		Pressed = false;
		
		if (Background_Pressed)
		{
			Background_Pressed._visible = false;
			if (Over && Background_RollOver)
				Background_RollOver._visible = true;
			else
				Background._visible = true;
		}
		
		OnRelease();
	}
	
	public function SetText(Text:String):Void
	{
		Textfield.text = Text;
		Textfield.setTextFormat(Textformat);
	}
}