/**
 * ...
 * @author Niccolo Abate
 */
class SpeechBubble
{
	
	private var Background:MovieClip;
	private var Textfield:TextField;
	private var Textformat:TextFormat;
	private var Life:Number = -1;
	public var Id:Number;
	
	public var Target_X:Number;
	public var Target_Y:Number;
	private var X_Temp:Number;
	private var Y_Temp:Number;
	private var XFlip:Number;
	private var YFlip:Number;
	public var Size:Number;
	
	private static var LastId:Number = 0;
	private static function GetNewId():Number
	{
		return LastId++;
	}
	
	public function SpeechBubble(x:Number, y:Number, size:Number)
	{
		Id = GetNewId();
		Target_X = x;
		Target_Y = y;
		Size = size;
		InitializeBackground();
		InitializeTextfield();
	}
	
	private function CalculateBackgroundCoords():Void
	{
		if (Target_X < Main.UsableScreen_X / 2)
			XFlip = 1;
		else
			XFlip = -1;
		if (Target_Y < Main.UsableScreen_Y / 2)
			YFlip = 1;
		else
			YFlip = -1;
		X_Temp = Target_X;
		Y_Temp = Target_Y;
	}
	private function InitializeBackground():Void
	{
		var BackgroundName:String = "SB_" + Id + "_Background";
		CalculateBackgroundCoords();
		Background = _root.attachMovie("lib.Bubble_1.png", BackgroundName, _root.getNextHighestDepth());
		Background._x = X_Temp;
		Background._y = Y_Temp;
		Background._xscale *= Size * XFlip;
		Background._yscale *= Size * YFlip;
	}
	private function UpdateBackground():Void
	{
		CalculateBackgroundCoords();
		Background._x = X_Temp;
		Background._y = Y_Temp;
		Background._xscale = 100 * Size * XFlip;
		Background._yscale = 100 * Size * YFlip;
	}
	
	private function CalculateTextfieldCoords():Void
	{
		if(XFlip == 1)
			X_Temp = Target_X + BubbleInfo.BUBBLE_1_TAIL_X;
		else
			X_Temp = Target_X - BubbleInfo.BUBBLE_1_X; // + BubbleInfo.BUBBLE_1_TAIL_X;
		if(YFlip == 1)
			Y_Temp = Target_Y + BubbleInfo.BUBBLE_1_TAIL_Y;
		else
			Y_Temp = Target_Y - BubbleInfo.BUBBLE_1_Y; // - BubbleInfo.BUBBLE_1_TAIL_Y;
	}
	private function InitializeTextfield():Void
	{
		var TextfieldName:String = "SB_" + Id + "_Textfield";
		CalculateTextfieldCoords();
		var Width:Number = BubbleInfo.BUBBLE_1_X - BubbleInfo.BUBBLE_1_TAIL_X;
		var Height:Number = BubbleInfo.BUBBLE_1_Y - BubbleInfo.BUBBLE_1_TAIL_Y;
		Textfield = _root.createTextField(TextfieldName, _root.getNextHighestDepth(), X_Temp, Y_Temp, Width, Height);
		Textfield.multiline = true;
		Textfield.wordWrap = true;
		Textfield.embedFonts = true;
		//Textfield.type = "input";
		Textformat = new TextFormat();
		Textformat.font = "fonts.arial.ttf";
		Textformat.size = 25;
		Textformat.color = 0x000000;
		Textformat.underline = false;
		Textformat.italic = false;
		Textformat.align = "center";
		Textformat.bold = false;
		Textfield.text = "This is my first test field object text.";
		Textfield.setTextFormat(Textformat);
	}
	private function UpdateTextfield():Void
	{
		CalculateTextfieldCoords();
		Textfield._x = X_Temp;
		Textfield._y = Y_Temp;
	}
	
	
	public function Update(x:Number, y:Number, size:Number):Void
	{
		Target_X = x;
		Target_Y = y;
		Size = size;
		UpdateBackground();
		UpdateTextfield();
	}
	
	public function ChangeText(Text:String):Void
	{
		Textfield.text = Text;
		Textfield.setTextFormat(Textformat);
	}
	
	public function Destroy():Void
	{
		Textfield.removeTextField();
		Background.removeMovieClip();
		delete this;
	}
}