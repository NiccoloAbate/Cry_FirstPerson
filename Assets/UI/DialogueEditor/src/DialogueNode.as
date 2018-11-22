import DialogueNodes.InfoNode;
/**
 * ...
 * @author Niccolo Abate
 */
class DialogueNode
{
	
	static public var NODETYPE_START:Number = 0;
	static public var NODETYPE_BRANCH:Number = 1;
	static public var NODETYPE_END:Number = 2;
	static public function IsValidNodeType(Type:Number):Boolean
	{
		return (Type == NODETYPE_START || Type == NODETYPE_BRANCH || Type == NODETYPE_END);
	}
	public var NodeType:Number;
	
	static private var LastId:Number = -1;
	public var Index:Number;
	
	public var Background:MovieClip;
	static private var Background_Col:Number = 0x555555;
	static private var Background_BorderSize:Number = 2;
	static private var Background_BorderCol:Number = 0x333333;
	
	public var Events:MovieClip;
	private var bIsDragging:Boolean = false;
	
	static private var TextSize:Number = 12;
	static private var TextCol:Number = 0xffffff;
	static private var AltTextCol:Number = 0xcccccc;
	
	public var Name:String;
	public var X:Number;
	public var Y:Number;
	public var Parent:MovieClip;
	
	public var W:Number;
	public var H:Number;
	
	private var Elements:Array;
	private var CurrentConnector:MovieClip;
	private var Connectors:Array;
	static private var LastConnectorId:Number = -1;
	
	static public var InTextField:Boolean = false;
	
	public var Responses:Array;
	static private var LastNewNum:Number = -1;
	
	public function DialogueNode(nodeType:Number, x:Number, y:Number, parent:MovieClip) 
	{
		NodeType = nodeType;
		
		X = x;
		Y = y;
		if (parent)
			Parent = parent;
		else
			Parent = _root;
		
		Initialize_Common();
		if (NodeType == NODETYPE_START)
			Initialize_StartNode();
		else if (NodeType == NODETYPE_BRANCH)
			Initialize_BranchNode();
		else if (NodeType == NODETYPE_END)
			Initialize_EndNode();
		
		Initialize_Callbacks();
	}
	private function Initialize_Common():Void
	{
		Index = ++LastId;
		Name = "DialogueNode_" + Index;
		
		Elements = new Array();
		Connectors = new Array();
	}
	private function Initialize_StartNode():Void
	{
		W = 150;
		H = 60;
		Background = DrawUtils.DrawRect(Name + "_Background", X, Y, W, H, Background_Col, Background_BorderSize, Background_BorderCol, Parent);
		Background.Name_InputField = GiveInputTextField("StartName", 15, 15, Background._width - 60, 30, "Start_1");
		Background.Next_Output = GiveOutput("Next", 30, 30, 20, 1);
	}
	private function Initialize_BranchNode():Void
	{
		W = 200;
		H = 225;
		Background = DrawUtils.DrawRect(Name + "_Background", X, Y, W, H, Background_Col, Background_BorderSize, Background_BorderCol, Parent);
		Background.Name_InputField = GiveInputTextField("BranchName", 45, 15, Background._width - 60, 30, "Branch_1_1");
		
		GiveInputTextField("Text", 15, 60, Background._width - 30, 120, "");
		
		var THIS:DialogueNode = this;
		
		Responses = new Array();
		var NewResponse_Button:MyButton = GiveButton("NewResponse_Button", 15, 195, (Background._width - 60) / 2, 15, "+ Response");
		NewResponse_Button.OnPress = function() { THIS.GiveResponse(); }
		
		GiveInput("In", 15, 30, 20);
		Background.DefaultOutput = GiveOutput("Next", 90, 30, 20, 1);
	}
	private function Initialize_EndNode():Void
	{
		W = 150;
		H = 105;
		Background = DrawUtils.DrawRect(Name + "_Background", X, Y, W, H, Background_Col, Background_BorderSize, Background_BorderCol, Parent);
		Background.Name_InputField = GiveInputTextField("EndName", 45, 15, Background._width - 60, 30, "End_1");
		GiveInputTextField("EventName", 45, 60, Background._width - 60, 30, "End_Null");
		GiveInput("In", 15, 30, 20);
	}
	
	private function Initialize_Callbacks():Void
	{
		var THIS:DialogueNode = this;
		
		Events = _root.createEmptyMovieClip(Name + "_Events", _root.getNextHighestDepth());
		
		Events.onMouseDown = function() { THIS.OnPress(); }
		Events.onMouseUp = function() { THIS.OnRelease(); }
		Events.onMouseMove = function() { THIS.OnMouseMove(); }
	}
	private function OnPress():Void
	{
		if (Main.HasFocus())
			return;
		if (MouseHitTest(Background) && !ElementMouseHitTest())
		{
			var NodeEditor:DialogueNodeEditor = Main.MAIN.GetActiveNodeEditor();
			var ChangeBase:Object = DialogueNode.TranslationBase(NodeEditor.Background, NodeEditor.NodePane);
			var BaseX:Number = ChangeBase.X;
			var BaseY:Number = ChangeBase.Y;
			var OffsetX:Number = Background._width / 2;
			var OffsetY:Number = Background._height / 2;
			Background.startDrag(false, BaseX + 0 - OffsetX, BaseY + NodeEditor.Background._height - OffsetY, BaseX + NodeEditor.Background._width - OffsetX, BaseY + 0 - OffsetY);
			bIsDragging = true;
		}
		
		var Output:MovieClip = OutputMouseHit();
		if (Output)
		{
			StartOutputDrag(Output);
		}
	}
	private function OnRelease():Void
	{
		if (bIsDragging)
		{
			bIsDragging = false;
			Background.stopDrag();
		}
		
		if (CurrentConnector)
			StopOutputDrag();
	}
	private function OnMouseMove():Void
	{
		var NodeEditor:DialogueNodeEditor = Main.MAIN.GetActiveNodeEditor();
		
		if (bIsDragging)
		{
			var XChange:Number = Background._x - X;
			var YChange:Number = Background._y - Y;
			X = Background._x;
			Y = Background._y;
			
			UpdateConnectors(XChange, YChange);
		}
		
		if (CurrentConnector)
			UpdateOutputDrag();
	}
	
	private function GiveTag(name:String, x:Number, y:Number, w:Number, Text:String, _parent:MovieClip):MovieClip
	{
		if (!_parent)
			_parent = Background;
		
		var Element:MovieClip = _parent.createEmptyMovieClip(_parent._name + "_Tag_" + name + "_Background", _parent.getNextHighestDepth());
		Element._x = x;
		Element._y = y;
		
		var Textformat:TextFormat = new TextFormat();
		Textformat.font = "fonts.arial.ttf";
		Textformat.size = TextSize;
		Textformat.color = AltTextCol;
		Textformat.underline = false;
		Textformat.italic = false;
		Textformat.align = "left";
		Textformat.bold = false;
		
		var Textfield:TextField = Element.createTextField(Name + "_Tag_" + name + "_Textfield", Element.getNextHighestDepth(), 0, 0, w, 15);
		Textfield.multiline = false;
		Textfield.wordWrap = false;
		Textfield.embedFonts = true;
		Textfield.setNewTextFormat(Textformat);
		Textfield.text = Text;
		Textfield.setTextFormat(Textformat);
		
		Element.Textfield = Textfield;
		Element.Type = "Tag";
		
		Elements.push(Element);
		
		return Element;
	}
	private function GiveInputTextField(name:String, x:Number, y:Number, w:Number, h:Number, DefaultText:String, _parent:MovieClip):MovieClip
	{
		if (!_parent)
			_parent = Background;
		
		var Element:MovieClip = DrawUtils.DrawRect(_parent._name + "_TextInput_" + name + "_Background", x, y + 15, w, h - 15, Background_Col + 0x444444, Background_BorderSize, Background_BorderCol + 0x444444, _parent);
		
		var Textformat:TextFormat = new TextFormat();
		Textformat.font = "fonts.arial.ttf";
		Textformat.size = TextSize;
		Textformat.color = AltTextCol;
		Textformat.underline = false;
		Textformat.italic = false;
		Textformat.align = "left";
		Textformat.bold = false;
		
		var NameField:TextField = Element.createTextField(_parent._name + "_TextInput_" + name + "_NameField", Element.getNextHighestDepth(), 0, -15, w, 15);
		NameField.multiline = false;
		NameField.wordWrap = false;
		NameField.embedFonts = true;
		NameField.setNewTextFormat(Textformat);
		NameField.text = name;
		NameField.setTextFormat(Textformat);
		
		Textformat.color = TextCol;
		
		var Textfield:TextField = Element.createTextField(_parent._name + "_TextInput_" + name + "_Textfield", Element.getNextHighestDepth(), 0, 0, w, h - 15);
		Textfield.multiline = false;
		Textfield.wordWrap = true;
		Textfield.embedFonts = true;
		Textfield.type = "input";
		Textfield.setNewTextFormat(Textformat);
		Textfield.text = DefaultText;
		Textfield.setTextFormat(Textformat);
		
		Textfield.onSetFocus = function()
		{
			DialogueNode.InTextField = true;
		}
		Textfield.onKillFocus = function()
		{
			DialogueNode.InTextField = false;
		}
		
		Element.Type = "TextInput";
		Element.Namefield = NameField;
		Element.Textfield = Textfield;
		
		Elements.push(Element);
		
		return Element;
	}
	private function GiveButton(name:String, x:Number, y:Number, w:Number, h:Number, Text:String, _parent:MovieClip):MyButton
	{
		if (!_parent)
			_parent = Background;
		
		var NewButton:MyButton = new MyButton(_parent._name + "_Button_" + name, x, y, _parent);
		
		var Col:Number = Background_Col + 0x444444;
		var Thickness:Number = 1;
		var BorderCol:Number = Background_BorderCol + 0x444444;
		
		var Background_Info:Object = NewButton.MakeRectInfo("_Background", w, h, Col, Thickness, BorderCol);
		var RollOver_Info:Object = NewButton.MakeRectInfo("_RollOver", w, h, Col - 0x111111, Thickness, BorderCol - 0x111111);
		var Pressed_Info:Object = NewButton.MakeRectInfo("_Pressed", w, h, Col - 0x222222, Thickness + 1, BorderCol - 0x222222);
		NewButton.Initialize_Rects(Background_Info, RollOver_Info, Pressed_Info);
		if (Text)
			NewButton.GiveText(Text, AltTextCol, TextSize);
		
		NewButton.Background_MC.Type = "Button";
		NewButton.Background_MC.ParentButton = NewButton;
		
		Elements.push(NewButton.Background_MC);
		
		return NewButton;
	}
	private function GiveOutput(name:String, y:Number, w:Number, h:Number, TotalConnectors:Number):MovieClip
	{
		var Output:MovieClip = DrawUtils.DrawRect(Name + "_Output_" + name, W - (w / 2), y, w, h, Background_Col + 0x444444, Background_BorderSize, Background_BorderCol + 0x444444, Background);
		
		var Textfield:TextField = Output.createTextField(Name + "_Output_" + name + "_Textfield", Output.getNextHighestDepth(), -30, 0, w + 30, h);
		Textfield.multiline = false;
		Textfield.wordWrap = true;
		Textfield.embedFonts = true;
		var Textformat:TextFormat = new TextFormat();
		Textformat.font = "fonts.arial.ttf";
		Textformat.size = TextSize;
		Textformat.color = TextCol;
		Textformat.underline = false;
		Textformat.italic = false;
		Textformat.align = "right";
		Textformat.bold = false;
		Textfield.setNewTextFormat(Textformat);
		Textfield.text = name;
		Textfield.setTextFormat(Textformat);
		
		if (!TotalConnectors)
			TotalConnectors = -1;
		
		Output.Type = "Output";
		Output.Textfield = Textfield;
		Output.ParentNode = this;
		Output.TotalConnectors = TotalConnectors;
		Output.NumConnectors = 0;
		Output.Connectors = new Array();
		
		
		Elements.push(Output);
		
		return Output;
	}
	private function GiveInput(name:String, y:Number, w:Number, h:Number, TotalConnectors:Number):MovieClip
	{
		var Input:MovieClip = DrawUtils.DrawRect(Name + "_Input_" + name, (w / 2) * -1, y, w, h, Background_Col + 0x444444, Background_BorderSize, Background_BorderCol + 0x444444, Background);
		
		var Textfield:TextField = Input.createTextField(Name + "_Input_" + name + "_Textfield", Input.getNextHighestDepth(), -30, 0, w + 30, h);
		Textfield.multiline = false;
		Textfield.wordWrap = true;
		Textfield.embedFonts = true;
		var Textformat:TextFormat = new TextFormat();
		Textformat.font = "fonts.arial.ttf";
		Textformat.size = TextSize;
		Textformat.color = TextCol;
		Textformat.underline = false;
		Textformat.italic = false;
		Textformat.align = "right";
		Textformat.bold = false;
		Textfield.setNewTextFormat(Textformat);
		Textfield.text = name;
		Textfield.setTextFormat(Textformat);
		
		if (!TotalConnectors)
			TotalConnectors = -1;
		
		Input.Type = "Input";
		Input.Textfield = TextField;
		Input.ParentNode = this;
		Input.TotalConnectors = TotalConnectors;
		Input.NumConnectors = 0;
		Input.Connectors = new Array();
		
		Elements.push(Input);
		
		return Input;
	}
	
	private function GiveResponse():MovieClip
	{
		var _Y:Number = Background._height - 15;
		var Id:Number = Responses.length;
		var _Id_:Number = ++LastNewNum + 10;
		var name:String = Name + "_Response_" + _Id_;
		var w:Number = W - 30;
		var h:Number = 100;
		var Response:MovieClip = DrawUtils.DrawRect(name + "_Background", 15, _Y + 15,  w, h, Background_Col, Background_BorderSize, Background_BorderCol, Background);
		
		var Tag:MovieClip = GiveTag("NameTag", (Response._width / 2) - 40, 0, 80, "Response_" + Id, Response);
		Tag.Textfield.align = "center";
		var DeleteButton:MyButton = GiveButton("DeleteResponse", Response._width - 15, 0, 15, 15, "(-)", Response);
		var THIS:DialogueNode = this;
		DeleteButton.OnPress = function()
		{
			for (var i:Number = Response.Id + 1; i < THIS.Responses.length; i++)
			{
				THIS.Responses[i]._y -= (h + 15);
				THIS.Responses[i].Output._y -= (h + 15);
				THIS.Responses[i].Id--;
				THIS.Responses[i].NameTag.Textfield.text = "Response_" + THIS.Responses[i].Id;
			}
			THIS.H -= (h + 15);
			DrawUtils.ReDrawRect(THIS.Background, THIS.Background._x, THIS.Background._y, THIS.W, THIS.H, DialogueNode.Background_Col, DialogueNode.Background_BorderSize, DialogueNode.Background_BorderCol);
			THIS.DestroyElement(Response);
			if (THIS.Responses.length == 0)
			{
				THIS.Background.DefaultOutput = THIS.GiveOutput("Next", 90, 30, 20, 1);
			}
		};
		var TextInput:MovieClip = GiveInputTextField("Text", 15, 15, Response._width - 30, Response._height - 30, "", Response);
		
		H += h + 15;
		DrawUtils.ReDrawRect(Background, Background._x, Background._y, W, H, Background_Col, Background_BorderSize, Background_BorderCol);
		
		var Output:MovieClip = GiveOutput("Next_" + _Id_, Response._y + 30, 30, 20, 1);
		Output.Textfield.text = "Next";
		
		Response.Type = "Response";
		Response.Id = Id;
		Response.NameTag = Tag;
		Response.DeleteButton = DeleteButton;
		Response.TextInput = TextInput;
		Response.Output = Output;
		
		Elements.push(Response);
		Responses.push(Response);
		
		if (Id == 0)
		{
			DestroyElement(Background.DefaultOutput);
			Background.DefaultOutput = null;
		}
		
		return Response;
	}
	
	public function Save():Void
	{
		if (NodeType == NODETYPE_START)
		{
			var name:String = Background.Name_InputField.Textfield.text;
			var Next_Connector:Object = Background.Next_Output.Connectors[0];
			var Next_HitNode:DialogueNode = Next_Connector.Input.ParentNode;
			var Next:String = Next_HitNode.Background.Name_InputField.Textfield.text;
			fscommand("Save_Node", new Array(NodeType, name, Next));
		}
	}
	public function Destroy():Void
	{
		DeleteConnectors(Connectors);
		for (var i:Number = 0; i < Elements.length; i++)
		{
			DestroyElement(Elements[i]);
		}
		Background.removeMovieClip();
	}
	
	private function DestroyElement(Element:MovieClip):Void
	{
		SpliceElement(Elements, Element);
		
		if (Element.Type == "Response")
		{
			SpliceElement(Responses, Element);
			DestroyElement(Element.NameTag);
			DestroyElement(Element.DeleteButton);
			DestroyElement(Element.TextInput);
			DestroyElement(Element.Output);
			Element.removeMovieClip();
			return;
		}
		if (Element.Type == "Button")
		{
			Element.ParentButton.Destroy();
			return;
		}
		if (Element.Type == "Output")
		{
			DeleteConnectors(Element.Connectors);
		}
		if (Element.Type == "Input")
		{
			DeleteConnectors(Element.Connectors);
		}
		
		Element.removeMovieClip();
	}
	
	private function MouseHitTest(MC:MovieClip):Boolean
	{
		return MC.hitTest(_root._xmouse, _root._ymouse)
	}
	public function ElementMouseHitTest():Boolean
	{
		for (var i:Number = 0; i < Elements.length; i++)
		{
			if (MouseHitTest(Elements[i]))
				return true;
		}
		return false;
	}
	public function TextInputMouseHitTest():Boolean
	{
		for (var i:Number = 0; i < Elements.length; i++)
		{
			var Element:MovieClip = Elements[i];
			if (MouseHitTest(Element))
			{
				if(Element.Type == "TextInput")
					return true;
			}
		}
		return false;
	}
	private function OutputMouseHit():MovieClip
	{
		for (var i:Number = 0; i < Elements.length; i++)
		{
			if (MouseHitTest(Elements[i]))
			{
				if (Elements[i].Type == "Output")
					return Elements[i];
			}
		}
		return null;
	}
	public function InputMouseHit():MovieClip
	{
		for (var i:Number = 0; i < Elements.length; i++)
		{
			if (MouseHitTest(Elements[i]))
			{
				if (Elements[i].Type == "Input")
					return Elements[i];
			}
		}
		return null;
	}
	
	static public function DrawConnector(Name:String, StartX:Number, StartY:Number, EndX:Number, EndY:Number, Col:Number, Thickness:Number, Parent:MovieClip):MovieClip
	{
		var Line:MovieClip;
		if (Parent)
			Line = Parent.createEmptyMovieClip(Name, Parent.getNextHighestDepth());
		else
			Line = _root.createEmptyMovieClip(Name, _root.getNextHighestDepth());
		
		var MidX:Number = StartX + ((EndX - StartX) / 2);
		var MidY:Number = StartY + ((EndY - StartY) / 2);
		
		Line.lineStyle(Thickness, Col, 100);
		Line.moveTo(StartX, StartY);
		Line.curveTo(MidX, StartY, MidX, MidY);
		Line.moveTo(MidX, MidY);
		Line.curveTo(MidX, EndY, EndX, EndY);
		
		Line.StartX = StartX;
		Line.StartY = StartY;
		
		return Line;
	}
	private function StartOutputDrag(Output:MovieClip):Void
	{
		if (Output.TotalConnectors != -1 && Output.NumConnectors >= Output.TotalConnectors)
			return;
		
		var NodeEditor:DialogueNodeEditor = Main.MAIN.GetActiveNodeEditor();
		var StartX:Number = NodeEditor.NodePane._xmouse - Output._xmouse;
		StartX += (Output._width / 2) - 4;
		var StartY:Number = NodeEditor.NodePane._ymouse - Output._ymouse;
		StartY += Output._height / 2;
		
		CurrentConnector = NodeEditor.NodePane.createEmptyMovieClip(Output._name + "_Connector", NodeEditor.NodePane.getNextHighestDepth());
		CurrentConnector.StartX = StartX;
		CurrentConnector.StartY = StartY;
		CurrentConnector.Output = Output;
	}
	private function StopOutputDrag():Void
	{
		var NodeEditor:DialogueNodeEditor = Main.MAIN.GetActiveNodeEditor();
		
		var InputHit:MovieClip = NodeEditor.NodeInputMouseHit();
		if (InputHit)
		{
			var name:String = CurrentConnector._name;
			var Output:MovieClip = CurrentConnector.Output;
			
			var StartX:Number = CurrentConnector.StartX;
			var StartY:Number = CurrentConnector.StartY;
			var EndX:Number = NodeEditor.NodePane._xmouse - InputHit._xmouse;
			EndX += 4;
			var EndY:Number = NodeEditor.NodePane._ymouse - InputHit._ymouse;
			EndY += InputHit._height / 2;
		
			CurrentConnector.removeMovieClip();
			CurrentConnector = DrawConnector(name, StartX, StartY, EndX, EndY, 0xffffff, 3, NodeEditor.NodePane);
			
			/////////
			
			Output.Connectors.push(CurrentConnector);
			InputHit.Connectors.push(CurrentConnector);
			
			CurrentConnector.Output = Output;
			CurrentConnector.Input = InputHit;
			CurrentConnector.EndX = EndX;
			CurrentConnector.EndY = EndY;
			CurrentConnector.Id = ++LastConnectorId;
			
			Output.NumConnectors++;
			InputHit.NumConnectors++;
			
			Connectors.push(CurrentConnector);
			InputHit.ParentNode.Connectors.push(CurrentConnector);
			
			CurrentConnector = null;
			return;
		}
		
		CurrentConnector.removeMovieClip();
		CurrentConnector = null;
	}
	private function UpdateOutputDrag():Void
	{
		var NodeEditor:DialogueNodeEditor = Main.MAIN.GetActiveNodeEditor();
		
		var name:String = CurrentConnector._name;
		var Output:MovieClip = CurrentConnector.Output;
		
		var StartX:Number = CurrentConnector.StartX;
		var StartY:Number = CurrentConnector.StartY;
		var EndX:Number = NodeEditor.NodePane._xmouse;
		var EndY:Number = NodeEditor.NodePane._ymouse;
		
		CurrentConnector.removeMovieClip();
		CurrentConnector = DrawConnector(name, StartX, StartY, EndX, EndY, 0xffffff, 3, NodeEditor.NodePane);
		CurrentConnector.Output = Output;
	}
	public function UpdateConnectors(XChange:Number, YChange:Number):Void
	{
		if (Connectors.length == 0)
			return;
		var NodeEditor:DialogueNodeEditor = Main.MAIN.GetActiveNodeEditor();
		for (var i:Number = 0; i < Connectors.length; i++)
		{
			var Connector:MovieClip = Connectors[i];
			var name:String = Connector._name;
			var StartX:Number = Connector.StartX;
			var StartY:Number = Connector.StartY;
			var EndX:Number = Connector.EndX;
			var EndY:Number = Connector.EndY;
			var Input:MovieClip = Connector.Input;
			var Output:MovieClip = Connector.Output;
			
			var InputParentNode:DialogueNode = Input.ParentNode;
			var OutputParentNode:DialogueNode = Output.ParentNode;
			if (InputParentNode == this)
			{
				EndX += XChange;
				EndY += YChange;
			}
			else if (OutputParentNode == this)
			{
				StartX += XChange;
				StartY += YChange;
			}
			
			Connector.removeMovieClip();
			Connector = DrawConnector(name, StartX, StartY, EndX, EndY, 0xffffff, 3, NodeEditor.NodePane);
			
			Connector.EndX = EndX;
			Connector.EndY = EndY;
			Connector.Input = Input;
			Connector.Output = Output;
			
			Connectors[i] = Connector;
		}
	}
	
	private function DeleteConnectors(connectors:Array):Void
	{
		for (var i:Number = 0; i < connectors.length; i++)
		{
			SpliceElement(Connectors, connectors[i]);
			connectors[i].removeMovieClip();
		}
	}
	private function DeleteConnector(Connector:MovieClip):Void
	{
		SpliceElement(Connectors, Connector);
		Connector.removeMovieClip();
	}
	
	static private function SpliceElement(Arr:Array, Element:Object):Void
	{
		for (var i:Number = 0; i < Arr.length; i++)
		{
			if (Element == Arr[i])
			{
				Arr.splice(i, 1);
				return;
			}
		}
	}
	static private function TranslationBase(MC:MovieClip, NewParentSpace:MovieClip):Object
	{
		var Base:Object = new Object();
		Base.X = NewParentSpace._xmouse - MC._xmouse;
		Base.Y = NewParentSpace._ymouse - MC._ymouse;
		return Base;
	}
	
}