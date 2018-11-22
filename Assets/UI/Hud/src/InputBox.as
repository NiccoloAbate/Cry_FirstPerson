/**
 * ...
 * @author Niccolo Abate
 */
class InputBox
{
	
	public var Responses:Array;
	public var Format:TextFormat;
	public function InputBox() 
	{
		InitializeFormat();
	}
	
	public function InitializeFormat():Void
	{
		Format = new TextFormat;
		Format.font = "fonts.arial.ttf";
		Format.size = 22;
		Format.color = 0x000000;
		Format.underline = false;
		Format.italic = false;
		Format.align = "left";
		Format.bold = false;
	}
	public function ClearResponses():Void
	{
		for (var i:Number = 0; i < Responses.length; i++)
		{
			Responses[i].Destroy();
		}
		delete Responses;
	}
	public function SetResponseNum(Num:Number):Void
	{
		ClearResponses();
		Responses = new Array(Num);
	}
	public function OnRollOver(Index:Number):Void
	{
	}
	public function OnRollOut(Index:Number):Void
	{
	}
	public function OnPress(Index:Number):Void
	{
		fscommand("Dialogue_Response", Index);
		//Responses[Index].SetText("Tiddies " + Index);
	}
	public function OnRelease(Index:Number):Void
	{
	}
	public function SetResponse(Index:Number, Text:String):Void
	{
		var Step:Number = Responses.length - Index;
		var X:Number = (ScreenInfo.X - ResponseInfo.X) / 2;
		var Y:Number = ScreenInfo.Y - ScreenInfo.BORDERSIZE - (ResponseInfo.Y * Step) - (ResponseInfo.BORDERSIZE * Step);
		
		var Response:MyButton = new MyButton;
		Response.Initialize("Response_" + Index, X, Y, ResponseInfo.X, ResponseInfo.Y,
			ResponseInfo.BACKGROUND_FILE, ResponseInfo.BACKGROUND_OVER_FILE, ResponseInfo.BACKGROUND_PRESSED_FILE,
			Text, Format, ResponseInfo.TEXT_BORDERSIZE);
		Response.Id = Index;
		
		Response.OnPress = function()
		{
			Main.MAIN.inputBox.OnPress(Response.Id);
		}
		
		Responses[Index] = Response;
	}
	
}

		/*
		var DepthStep:Number = Responses.length - Index;
		var X:Number = (ScreenInfo.X - ResponseInfo.X) / 2;
		var Y:Number = ScreenInfo.Y - ScreenInfo.BORDERSIZE - (ResponseInfo.Y * DepthStep) - (ResponseInfo.BORDERSIZE * DepthStep);
		var Response:MovieClip = _root.attachMovie(ResponseInfo.CLEAR_FILE, "Response_" + Index, ResponseInfo.CLEAR_DEPTH);
		Response._x = X;
		Response._y = Y;
		Response.Id = Index;
		
		var Background:MovieClip = _root.attachMovie(ResponseInfo.BACKGROUND_FILE, "Response_" + Index + "_BG", ResponseInfo.BACKGROUND_DEPTH + DepthStep);
		Background._x = X;
		Background._y = Y;
		Response.Background = Background;
		
		var Background_Over:MovieClip = _root.attachMovie(ResponseInfo.BACKGROUND_OVER_FILE, "Response_" + Index + "_BG_Over", ResponseInfo.BACKGROUND_OVER_DEPTH + DepthStep);
		Background_Over._x = X;
		Background_Over._y = Y;
		Background_Over._visible = false;
		Response.Background_Over = Background_Over;
		
		var Textfield:TextField = _root.createTextField("Response_" + Index, ResponseInfo.TEXT_DEPTH + DepthStep, X, Y, ResponseInfo.X, ResponseInfo.Y);
		Textfield.multiline = true;
		Textfield.wordWrap = true;
		Textfield.embedFonts = true;
		Textfield.text = Text;
		Textfield.setTextFormat(Format);
		
		Response.Textfield = Textfield;
		
		Response.onRollOver = function()
		{
			Main.MAIN.inputBox.OnRollOver(Response.Id);
		}
		Response.onRollOut = function()
		{
			Main.MAIN.inputBox.OnRollOut(Response.Id);
		}
		
		Response.onPress = function()
		{
			Main.MAIN.inputBox.OnClicked(Response.Id);
		}
		
		Responses[Index] = Response;
		*/
		