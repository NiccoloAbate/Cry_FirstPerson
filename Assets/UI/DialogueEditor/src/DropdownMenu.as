/**
 * ...
 * @author Niccolo Abate
 */
class DropdownMenu
{
	
	var Name:String;
	var Events:MovieClip;
	var Elements:Array;
	
	var Parent:MovieClip;
	var X:Number;
	var Y:Number;
	
	static var Col:Number = 0x555555;
	static var Thickness:Number = 1;
	static var BorderCol:Number = 0x333333;
	static var W:Number = 120;
	static var H:Number = 14;
	static var TextSize:Number = 12;
	static var TextCol:Number = 0xdddddd;
	static private var DropdownTriangle_W:Number = 4;
	static private var DropdownTriangle_H:Number = 4;
	
	var bKeepFocus:Boolean = true;
	var bAutoDestroy:Boolean = true;
	
	public function DropdownMenu(name:String, x:Number, y:Number, parent:MovieClip) 
	{
		Initialize_Once();
		
		if (parent)
			Parent = parent;
		else
			Parent = _root;
		
		Elements = new Array();
		
		Name = name;
		X = x;
		Y = y;
		
		Events = _root.createEmptyMovieClip(name, _root.getNextHighestDepth());
		InitializeCallbacks();
		
		if (bKeepFocus)
		{
			Main.GiveFocus(this);
		}
	}
	static private var Initialized_Once:Boolean = false;
	private function Initialize_Once():Void
	{
		if (Initialized_Once)
			return;
		
	}
	private function InitializeCallbacks():Void
	{
		var THIS:DropdownMenu = this;
		Events.onMouseDown = function()
		{
			var bOutside:Boolean = true;
			for (var i:Number = 0; i < THIS.Elements.length; i++)
			{
				var Element:MyButton = THIS.Elements[i];
				if (THIS.MouseHitTest(Element.Background_MC))
				{
					bOutside = false;
					break;
				}
			}
			if (bOutside)
			{
				THIS.Destroy();
			}
		}
	}
	
	public function Destroy():Void
	{
		CloseDropdownElements();
		Events.removeMovieClip();
		for (var i:Number = 0; i < Elements.length; i++)
		{
			var Element:MyButton = Elements[i];
			Element.Destroy();
			Elements[i] = null;
		}
		Elements = null;
		Main.RemoveFocus(this);
		delete this;
	}
	private function MouseHitTest(MC:MovieClip):Boolean
	{
		return MC.hitTest(_root._xmouse, _root._ymouse)
	}
	public function MenuHitTest():Boolean
	{
		for (var i:Number = 0; i < Elements.length; i++)
		{
			var Element:MyButton = Elements[i];
			if (MouseHitTest(Element.Background_MC))
				return true;
		}
		return false;
	}
	
	private function GiveText(Element:MovieClip, Text:String):Void
	{
		var Textfield:TextField = _root.createTextField("ass", _root.getNextHighestDepth(), 0, 0, 0, 0);
		Textfield.multiline = true;
		Textfield.wordWrap = true;
		Textfield.embedFonts = true;
		//Textfield.type = "input";
		var Textformat:TextFormat = new TextFormat();
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
	public function AddElement(Text:String, Callback:Function):Number
	{
		var ElementIndex:Number = Elements.length;
		var ElementName:String = Name + "_Element_" + ElementIndex;
		var ElementX:Number = X;
		var ElementY:Number = Y + (H * ElementIndex);
		var Element:MyButton = new MyButton(ElementName, ElementX, ElementY, Parent);
		var Background_Info:Object = Element.MakeRectInfo("_Background", W, H, Col, Thickness, BorderCol);
		var RollOver_Info:Object = Element.MakeRectInfo("_RollOver", W, H, Col - 0x111111, Thickness, BorderCol - 0x111111);
		var Pressed_Info:Object = Element.MakeRectInfo("_Pressed", W, H, Col - 0x222222, Thickness + 1, BorderCol - 0x222222);
		Element.Initialize_Rects(Background_Info, RollOver_Info, Pressed_Info);
		Element.GiveText(Text, TextCol, TextSize);
		Element.OnPress = Callback;
		var THIS:DropdownMenu = this;
		Element.OnRelease = function()
		{
			THIS.Destroy();
		}
		
		Elements.push(Element);
		
		return ElementIndex;
	}
	static public function MakeElementInfo(Text:String, Callback:Function):Object
	{
		var Info:Object = new Object();
		Info.Type = 0; //Normal
		Info.Text = Text;
		Info.Callback = Callback;
		return Info;
	}
	public function AddElement_Dropdown(Text:String, DropdownElements:Array):Number
	{
		var ElementIndex:Number = Elements.length;
		var ElementName:String = Name + "_Element_" + ElementIndex;
		var ElementX:Number = X;
		var ElementY:Number = Y + (H * ElementIndex);
		var Element:MyButton = new MyButton(ElementName, ElementX, ElementY, Parent);
		var Background_Info:Object = Element.MakeRectInfo("_Background", W, H, Col, Thickness, BorderCol);
		var RollOver_Info:Object = Element.MakeRectInfo("_RollOver", W, H, Col - 0x111111, Thickness, BorderCol - 0x111111);
		var Pressed_Info:Object = Element.MakeRectInfo("_Pressed", W, H, Col - 0x222222, Thickness + 1, BorderCol - 0x222222);
		Element.Initialize_Rects(Background_Info, RollOver_Info, Pressed_Info);
		Element.GiveText(Text, TextCol, TextSize);
		GiveDropdownTriangle(Element);
		
		var THIS:DropdownMenu = this;
		Element.OnRollOver = function():Void
		{
			THIS.CloseDropdownElements();
			var Dropdown:DropdownMenu = new DropdownMenu(ElementName + "_Dropdown", ElementX + DropdownMenu.W, ElementY, THIS.Parent);
			Element.Background_MC.Dropdown = Dropdown;
			for (var i:Number = 0; i < DropdownElements.length; i++)
			{
				var ElementInfo:Object = DropdownElements[i];
				if (ElementInfo.Type == 0) // Normal
					Dropdown.AddElement(ElementInfo.Text, ElementInfo.Callback);
				if (ElementInfo.Type == 1) // Dropdown
					Dropdown.AddElement_Dropdown(ElementInfo.Text, ElementInfo.DropdownElements);
			}
		}
		Element.OnRollOut = function():Void
		{
			var Dropdown:DropdownMenu = Element.Background_MC.Dropdown;
			if (!Dropdown.MenuHitTest())
			{
				Dropdown.Destroy();
				Element.Background_MC.Dropdown = null;
			}
		}
		
		Elements.push(Element);
		
		return ElementIndex;
	}
	static public function MakeElement_DropdownInfo(Text:String, DropdownElements:Array):Object
	{
		var Info:Object = new Object();
		Info.Type = 1; //Dropdown
		Info.Text = Text;
		Info.DropdownElements = DropdownElements;
		return Info;
	}
	private function GiveDropdownTriangle(Element:MyButton):MovieClip
	{
		var Tri:MovieClip = Element.Background_MC.createEmptyMovieClip(Element.Name + "_DropdownTriangle", Element.Background_MC.getNextHighestDepth());
		Tri._x = W - (4 + DropdownTriangle_W);
		Tri._y = (H - DropdownTriangle_H) / 2;
		Tri.lineStyle(0, TextCol, 100, true, "none", "round", "miter", 1);
		Tri.beginFill(TextCol);
		Tri.moveTo(0, 0);
		Tri.lineTo(DropdownTriangle_W, DropdownTriangle_H / 2);
		Tri.lineTo(0, DropdownTriangle_H);
		Tri.lineTo(0, 0);
		Tri.endFill();
		return Tri;
	}
	
	private function CloseDropdownElements():Void
	{
		for (var i:Number = 0; i < Elements.length; i++)
		{
			var Element:MyButton = Elements[i];
			if (Element.Background_MC.Dropdown)
			{
				Element.Background_MC.Dropdown.Destroy();
				Element.Background_MC.Dropdown = null;
			}
		}
	}
	
}