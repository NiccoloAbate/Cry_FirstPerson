/**
 * ...
 * @author Niccolo Abate
 */
class PopupMenu
{
	
	public var Background:MovieClip;
	static private var Background_Col:Number = 0x555555;
	static private var Background_BorderSize:Number = 2;
	static private var Background_BorderCol:Number = 0x333333;
	
	public var Top:MovieClip;
	private static var Top_H:Number = 30;
	
	private var Accept_Button:MyButton;
	private var Cancel_Button:MyButton;
	static private var Bottom_H:Number = 30;
	static private var Bottom_Buttons_W:Number = 100;
	
	public var Parent:MovieClip;
	public var Name:String;
	
	static private var TextSize:Number = 12;
	static private var TextCol:Number = 0xdddddd;
	
	public var Elements:Array;
	
	public var AcceptCheckFunction:Function;
	
	public function PopupMenu(name:String, X:Number, Y:Number, W:Number, H:Number, parent:MovieClip) 
	{
		if (parent)
			Parent = parent;
		else
			Parent = _root;
		
		Name = name;
		
		Background = DrawUtils.DrawRect(Name + "_Background", X, Y, W, H + Top_H + Bottom_H, Background_Col, Background_BorderSize, Background_BorderCol, Parent);
		Top = DrawUtils.DrawRect(Name + "_Top", 0, 0, W, Top_H, Background_Col + 0x222222, Background_BorderSize, Background_BorderCol, Background);
		
		Accept_Button = new MyButton(Name + "_Accept_Button", W - (Bottom_Buttons_W + 50), H + Top_H + 4, Background);
		var Background_Info:Object = Accept_Button.MakeRectInfo("_Background", Bottom_Buttons_W, Bottom_H - 8, Background_Col + 0x444444, Background_BorderSize, Background_BorderCol + 0x444444);
		var RollOver_Info:Object = Accept_Button.MakeRectInfo("_RollOver", Bottom_Buttons_W, Bottom_H - 8, Background_Col + 0x333333, Background_BorderSize, Background_BorderCol + 0x333333);
		var Pressed_Info:Object = Accept_Button.MakeRectInfo("_Pressed", Bottom_Buttons_W, Bottom_H - 8, Background_Col + 0x222222, Background_BorderSize, Background_BorderCol + 0x222222);
		Accept_Button.Initialize_Rects(Background_Info, RollOver_Info, Pressed_Info);
		Accept_Button.GiveText("Accept", TextCol, TextSize);
		
		Cancel_Button = new MyButton(Name + "_Cancel_Button", 50, H + Top_H + 4, Background);
		Background_Info = Cancel_Button.MakeRectInfo("_Background", Bottom_Buttons_W, Bottom_H - 8, Background_Col + 0x444444, Background_BorderSize, Background_BorderCol + 0x444444);
		RollOver_Info = Cancel_Button.MakeRectInfo("_RollOver", Bottom_Buttons_W, Bottom_H - 8, Background_Col + 0x333333, Background_BorderSize, Background_BorderCol + 0x333333);
		Pressed_Info = Cancel_Button.MakeRectInfo("_Pressed", Bottom_Buttons_W, Bottom_H - 8, Background_Col + 0x222222, Background_BorderSize, Background_BorderCol + 0x222222);
		Cancel_Button.Initialize_Rects(Background_Info, RollOver_Info, Pressed_Info);
		Cancel_Button.GiveText("Cancel", TextCol, TextSize);
		
		var THIS:PopupMenu = this;
		Accept_Button.OnRelease = function() { THIS.AcceptButton_OnRelease(); };
		Cancel_Button.OnRelease = function() { THIS.CancelButton_OnRelease(); };
		
		Elements = new Array();
		
		Main.GiveFocus(this);
	}
	public function Destroy():Void
	{
		Accept_Button.Destroy();
		Cancel_Button.Destroy();
		Top.removeMovieClip();
		Background.removeMovieClip();
		Main.RemoveFocus(this);
	}
	
	public function SetAcceptButtonText(Text:String):Void
	{
		Accept_Button.Textfield.text = Text;
	}
	//private function Initialize
	
	public function GiveInputField(name:String, X:Number, Y:Number, W:Number, H:Number, DefaultText:String):Number
	{
		var Element:MovieClip = DrawUtils.DrawRect(Name + "_TextInput_" + name + "_Background", X, Y + Top_H, W, H, Background_Col + 0x444444, Background_BorderSize, Background_BorderCol + 0x444444, Background);
		
		var Textfield:TextField = Element.createTextField(Name + "_TextInput_" + name + "_Textfield", Element.getNextHighestDepth(), 0, 0, W, H);
		Textfield.multiline = true;
		Textfield.wordWrap = true;
		Textfield.embedFonts = true;
		Textfield.type = "input";
		var Textformat:TextFormat = new TextFormat();
		Textformat.font = "fonts.arial.ttf";
		Textformat.size = TextSize;
		Textformat.color = TextCol;
		Textformat.underline = false;
		Textformat.italic = false;
		Textformat.align = "left";
		Textformat.bold = false;
		Textfield.setNewTextFormat(Textformat);
		Textfield.text = DefaultText;
		Textfield.setTextFormat(Textformat);
		
		Element.Type = "Text";
		Element.Textfield = Textfield;
		Textfield.DefaultText = DefaultText;
		var Index:Number = Elements.push(Element) - 1;
		Element.Index = Index;
		
		return Index;
	}
	
	private function CheckElements():Void
	{
		for (var i:Number = 0; i < Elements.length; i++)
		{
			if (Elements[i].Type == "Text")
			{
				var Element:MovieClip = Elements[i];
				CheckElementTextChanged(i);
			}
		}
	}
	private function CheckElementTextChanged(Index:Number):Void
	{
		var Element:MovieClip = Elements[Index];
		var Textfield:TextField = Element.Textfield;
		if (Textfield.text != Textfield.DefaultText)
		{
			trace(63);
		}
	}
	public function IsElementTextChanged(Index:Number):Boolean
	{
		var Element:MovieClip = Elements[Index];
		var Textfield:TextField = Element.Textfield;
		return (Textfield.text != Textfield.DefaultText);
	}
	
	// Callbacks
	public var OnAccept:Function;
	public var OnCancel:Function;
	private function AcceptButton_OnRelease():Void
	{
		if (AcceptCheckFunction)
		{
			var Valid:Boolean = AcceptCheckFunction();
			if (!Valid)
				return;
		}
		
		CheckElements();
		if(OnAccept)
			OnAccept();
		Destroy();
	}
	private function CancelButton_OnRelease():Void
	{
		CheckElements();
		if(OnCancel)
			OnCancel();
		Destroy();
	}
	//~Callbacks
	
}