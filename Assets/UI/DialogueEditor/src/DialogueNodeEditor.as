/**
 * ...
 * @author Niccolo Abate
 */
class DialogueNodeEditor
{
	var Index:Number;
	
	var Background:MovieClip;
	var Top:MovieClip;
	
	var DialogueNameBox:MovieClip;
	
	var NodePane:MovieClip;
	var NodeMask:MovieClip;
	
	var File_Button:MyButton;
	
	var Events:MovieClip;
	private var bPressed:Boolean = false;
	
	var dialogue:Dialogue;
	
	static private var __RightMouseDown__:Boolean = false;
	
	private var W:Number = 1080;
	private var H:Number = 720;
	static private var Background_Col:Number = 0x999999;
	static private var Background_BorderSize:Number = 5;
	static private var Background_BorderCol:Number = 0x777777;
	
	static private var TextSize:Number = 12;
	static private var TextCol:Number = 0xffffff;
	static private var AltTextCol:Number = 0xcccccc;
	
	static private var Top_H:Number = 30;
	static private var Top_Col:Number = 0x6072a4;
	
	public var Fullscreen:Boolean = false;
	
	private var NodeInFocus:DialogueNode;
	
	public function DialogueNodeEditor(index:Number)
	{
		Index = index;
		
		//Background = _root.attachMovie(DialogueNodeEditorInfo.BACKGROUND_FILE, "NodeEditor_" + Index + "_Background", _root.getNextHighestDepth());
		Background = DrawUtils.DrawRect("NodeEditor_" + Index + "_Background", 0, 0, W, H, Background_Col, Background_BorderSize, Background_BorderCol, _root);
		
		//Top = Background.attachMovie(DialogueNodeEditorInfo.TOP_FILE, "NodeEditor_" + Index + "_Top", Background.getNextHighestDepth());
		Top = DrawUtils.DrawRect("NodeEditor_" + Index + "_Top", 0, 0, W, Top_H, Top_Col, null, null, Background);
		
		NodePane = Background.createEmptyMovieClip("NodeEditor_" + Index + "_NodePane", Background.getNextHighestDepth());
		
		NodeMask = DrawUtils.DrawRect("NodeEditor_" + Index + "_NodeMask", 0, Top_H, W, H - Top_H, 0xffffff, null, null, Background);
		NodePane.setMask(NodeMask);
		
		File_Button = new MyButton("NodeEditor_" + Index + "_NewDialogue_Button", 0, 0, Top);
		File_Button.Initialize_FileNames(DialogueNodeEditorInfo.FILE_BUTTON_BACKGROUND_FILE);
		
		InitializeCallbacks();
		
		NewDialogue("Untitled");
		
		var THIS:DialogueNodeEditor = this;
		
		DialogueNameBox = DrawUtils.GiveInputTextField("Dialogue_Name", (W / 2) - 100, 0, 200, Top_H, "Untitled", Top);
		var Textfield:TextField = DialogueNameBox.Textfield;
		Textfield.onKillFocus = function()
		{
			THIS.dialogue.Name = Textfield.text;
			trace("Dialogue Name = " + THIS.dialogue.Name);
		}
	}
	
	private function MouseHitTest(MC:MovieClip):Boolean
	{
		return MC.hitTest(_root._xmouse, _root._ymouse)
	}
	private function TopElementMouseHitTest():Boolean
	{
		if (MouseHitTest(File_Button.Background_MC) || MouseHitTest(DialogueNameBox))
			return true;
		
		return false;
	}
	
	// Callbacks
	public function OnRightClick():Void
	{
		if (Main.HasFocus())
			return;
		var THIS:DialogueNodeEditor = this;
		
		if (!MouseHitTest(Top) && !NodeMouseHitTest())
		{
			var Dropdown:DropdownMenu = new DropdownMenu("Menu", Background._xmouse, Background._ymouse, Background);
			var Dropdown1Elements:Array = new Array();
			Dropdown1Elements.push(DropdownMenu.MakeElementInfo("Branch Node", function() 
				{
					var Translated:Object = DialogueNodeEditor.TranslatePos(THIS.Background, Dropdown.X, Dropdown.Y, THIS.NodePane);
					THIS.NewDialogueNode(DialogueNode.NODETYPE_BRANCH, Translated.X, Translated.Y);
				} ));
			Dropdown1Elements.push(DropdownMenu.MakeElementInfo("Start Node", function() 
				{
					var Translated:Object = DialogueNodeEditor.TranslatePos(THIS.Background, Dropdown.X, Dropdown.Y, THIS.NodePane);
					THIS.NewDialogueNode(DialogueNode.NODETYPE_START, Translated.X, Translated.Y); 
				} ));
			Dropdown1Elements.push(DropdownMenu.MakeElementInfo("End Node", function() 
				{
					var Translated:Object = DialogueNodeEditor.TranslatePos(THIS.Background, Dropdown.X, Dropdown.Y, THIS.NodePane);
					THIS.NewDialogueNode(DialogueNode.NODETYPE_END, Translated.X, Translated.Y);
				} ));
			Dropdown.AddElement_Dropdown("New Node", Dropdown1Elements);
			Dropdown.AddElement("Save", function() { THIS.SaveDialogue(); } );
			Dropdown.AddElement("Fullscreen", function() { THIS.SetFullscreen(!THIS.Fullscreen); } );
		}
		var NodeHit:DialogueNode = NodeMouseHit();
		if (NodeHit && !NodeHit.ElementMouseHitTest())
		{
			var Dropdown:DropdownMenu = new DropdownMenu("Menu", Background._xmouse, Background._ymouse, Background);
			Dropdown.AddElement("Copy", null);
			Dropdown.AddElement("Delete", function() { THIS.dialogue.DeleteNode(NodeHit); });
		}
		
	}
	private function OnPress():Void
	{
		if (Main.HasFocus())
			return;
		
		NodeInFocus = NodeMouseHit();
		
		if (MouseHitTest(Top) && !TopElementMouseHitTest() && !Fullscreen)
		{
			Background.startDrag(false, 0, 0, 1920 - Background._width, 1080 - Background._height);
		}
		
		if (!MouseHitTest(Top) && !TopElementMouseHitTest() && !NodeMouseHitTest())
		{
			NodePane.startDrag(false, -500000, 500000, 500000, -500000);
		}
	}
	private function OnRelease():Void
	{
		if (Main.HasFocus())
			return;
		Background.stopDrag();
		NodePane.stopDrag();
	}
	private function OnReleaseOutside():Void
	{
		OnRelease();
	}
	private function OnMouseWheel(Delta:Number):Void
	{
		if (Main.MAIN.IsActiveEditor(Index))
		{
			if (Key.isDown(Key.CONTROL))
				return;
			
			Zoom(Delta * -.01);
		}
	}
	
	private function InitializeCallbacks():Void
	{
		var THIS:DialogueNodeEditor = this;
		
		Events = _root.createEmptyMovieClip("NodeEditor_" + Index + "_Events", _root.getNextHighestDepth());
		Events.onMouseDown = function()
		{
			if (THIS.MouseHitTest(THIS.Background))
			{
				THIS.bPressed = true;
				THIS.OnPress();
			}
		}
		Events.onMouseUp = function()
		{
			if (THIS.bPressed)
			{
				THIS.bPressed = false;
				if (THIS.MouseHitTest(THIS.Background))
					THIS.OnRelease();
				else
					THIS.OnReleaseOutside();
			}
		}
		var WheelEvents:Object = new Object();
		WheelEvents.onMouseWheel = function(delta:Number) { THIS.OnMouseWheel(delta); }
		Mouse.addListener(WheelEvents);
		Events.WheelEvents = WheelEvents;
		
		if (!Main.CRYENGINE)
		{
			Events.onEnterFrame = function()
			{
				if (!DialogueNodeEditor.__RightMouseDown__)
				{
					if (Key.isDown(2))
					{
						DialogueNodeEditor.__RightMouseDown__ = true;
						THIS.OnRightClick();
					}
				}
				else
				{
					if (!Key.isDown(2))
					{
						DialogueNodeEditor.__RightMouseDown__ = false;
					}
				}
			}
		}
		
		var Menu:ContextMenu = new ContextMenu();
		Menu.hideBuiltInItems();
		Background.menu = Menu;
		
		InitializeFileButton();
	}
	
	private function InitializeFileButton():Void
	{
		var THIS:DialogueNodeEditor = this;
		File_Button.OnPress = function()
		{
			var Dropdown:DropdownMenu = new DropdownMenu("File_Dropdown", 0, THIS.File_Button.Background_MC._height, THIS.Top);
			Dropdown.AddElement("New Dialogue", function(){ THIS.OnNewDialogueButton_Pressed(); });
			Dropdown.AddElement("Load Dialogue", function(){ THIS.LoadDialogue(""); });
			Dropdown.AddElement("Save Dialogue", function(){ THIS.SaveDialogue(); });
			
			var Recent_Elements:Array = new Array();
			Recent_Elements.push(DropdownMenu.MakeElementInfo("Will Do Later", null));
			Dropdown.AddElement_Dropdown("Recent", Recent_Elements);
			
			var DialogueFolder_Elements:Array = new Array();
			if (!Main.MAIN.DialogueFileNames || Main.MAIN.DialogueFileNames.length == 0)
			{
				DialogueFolder_Elements.push(DropdownMenu.MakeElementInfo("__NO_FILES_FOUND__", null));
			}
			else
			{
				for (var i:Number = 0; i < Main.MAIN.DialogueFileNames.length; i++)
				{
					var DialogueName:String = Main.MAIN.DialogueFileNames[i];
					DialogueFolder_Elements.push(DropdownMenu.MakeElementInfo(DialogueName, function(){ THIS.LoadDialogue(DialogueName); }));
				}
			}
			Dropdown.AddElement_Dropdown("Dialogue Folder", DialogueFolder_Elements);
			
			/*
			 * Dropdown1Elements.push(DropdownMenu.MakeElementInfo("D 1", null));
			Dropdown1Elements.push(DropdownMenu.MakeElementInfo("D 2", null));
			var Dropdown1_1Elements:Array = new Array();
			Dropdown1_1Elements.push(DropdownMenu.MakeElementInfo("F 1", null));
			Dropdown1_1Elements.push(DropdownMenu.MakeElementInfo("F 2", null));
			Dropdown1Elements.push(DropdownMenu.MakeElement_DropdownInfo("Dropdown 2", Dropdown1_1Elements));
			Dropdown.AddElement_Dropdown("Dropdown 1", Dropdown1Elements);
			var Dropdown2Elements:Array = new Array();
			Dropdown2Elements.push(DropdownMenu.MakeElementInfo("H 1", null));
			Dropdown2Elements.push(DropdownMenu.MakeElementInfo("H 2", null));
			Dropdown.AddElement_Dropdown("Dropdown 2", Dropdown2Elements);
			*/
		}
	}
	private function OnNewDialogueButton_Pressed():Void
	{
		var Popup_W:Number = Background._width / 2;
		var Popup_H:Number = Background._height / 2;
		var Popup_X:Number = Background._width / 4;
		var Popup_Y:Number = Background._height / 4;
		var Popup:PopupMenu = new PopupMenu("NewDialogue_Popup", Popup_X, Popup_Y, Popup_W, Popup_H, Background);
		
		var DialogueName_Index:Number = Popup.GiveInputField("DialogueName", 50, 50, Popup.Background._width - 100, 30, "Dialogue Name");
		
		var THIS:DialogueNodeEditor = this;
		Popup.OnAccept = function():Void
		{
			var DialogueName:String = Popup.Elements[DialogueName_Index].Textfield.text;
			trace(DialogueName);
			THIS.NewDialogue(DialogueName);
		}
		Popup.AcceptCheckFunction = function():Boolean
		{
			return Popup.IsElementTextChanged(DialogueName_Index);
		}
	}
	//~Callbacks
	
	private function Zoom(Amount:Number):Void
	{
		Amount += 1;
		var OldX:Number = NodePane._xmouse;
		var OldY:Number = NodePane._ymouse;
		var AdjustedX:Number = OldX * Amount;
		var AdjustedY:Number = OldY * Amount;
		var OffsetX:Number = OldX - AdjustedX;
		var OffsetY:Number = OldY - AdjustedY;
		/*
		if (Amount > 1  && (Background._width * Amount) >= (1920 - DialogueNodeEditorInfo.ZOOMOFFSET))
		{
			return;
		}
		if (Amount < 1 && (Background._width * Amount) <= DialogueNodeEditorInfo.MINIMUMWIDTH)
			return;
		*/
		
		NodePane._xscale *= Amount;
		NodePane._yscale *= Amount;
		NodePane._x += OffsetX;
		NodePane._y += OffsetY;
	}
	
	public function SetFullscreen(fullscreen:Boolean):Void
	{
		if (fullscreen == Fullscreen)
			return;
		else
			Fullscreen = fullscreen;
		
		if (Fullscreen)
		{
			W = 1920;
			H = 1080;
			Background.OldX = Background._x;
			Background.OldY = Background._y;
			Background._x = 0;
			Background._y = 0;
		}
		else
		{
			W = 1080;
			H = 720;
			Background._x = Background.OldX;
			Background._y = Background.OldY;
			Background.OldX = null;
			Background.OldY = null;
		}
		DrawUtils.ReDrawRect(Background, Background._x, Background._y, W, H, Background_Col, Background_BorderSize, Background_BorderCol);
		DrawUtils.ReDrawRect(Top, 0, 0, W, Top_H, Top_Col, null, null);
		
		DrawUtils.ReDrawRect(NodeMask, 0, Top_H, W, H - Top_H, 0xffffff, null, null);
	}
	
	// Dialogue
	public function NewDialogue(Name:String):Dialogue
	{
		if (dialogue)
		{
			if (dialogue.Nodes.length > 0)
			{
				dialogue.Save();
				dialogue.Destroy();
				delete dialogue;
			}
		}
		dialogue = new Dialogue;
		dialogue.Name = Name;
		return dialogue;
	}
	public function LoadDialogue(Name:String):Void
	{
		trace("Loading: " + Name); 
	}
	public function SaveDialogue():Void
	{
		dialogue.Save();
	}
	
	public function NewDialogueNode(NodeType:Number, X:Number, Y:Number):DialogueNode
	{
		if (!DialogueNode.IsValidNodeType(NodeType))
			return null;
		
		//var X:Number = NodePane._xmouse;
		//var Y:Number = NodePane._ymouse;
		
		var Node:DialogueNode = new DialogueNode(NodeType, X, Y, NodePane);
		dialogue.Nodes.push(Node);
		
		return Node;
	}
	private function NodeMouseHitTest():Boolean
	{
		for (var i:Number = 0; i < dialogue.Nodes.length; i++)
		{
			var Node:DialogueNode = dialogue.Nodes[i];
			if (MouseHitTest(Node.Background))
				return true;
		}
		return false;
	}
	private function NodeMouseHit():DialogueNode
	{
		for (var i:Number = 0; i < dialogue.Nodes.length; i++)
		{
			var Node:DialogueNode = dialogue.Nodes[i];
			if (MouseHitTest(Node.Background))
				return Node;
		}
		return null;
	}
	public function NodeInputMouseHit():MovieClip
	{
		for (var i:Number = 0; i < dialogue.Nodes.length; i++)
		{
			var Node:DialogueNode = dialogue.Nodes[i];
			var InputHit:MovieClip = Node.InputMouseHit();
			if (InputHit)
				return InputHit;
		}
		return null;
	}
	//~Dialouge
	
	static private function TranslateMCPos(MC:MovieClip, NewParentSpace:MovieClip):Object
	{
		var Pos:Object = new Object();
		var BaseX:Number = NewParentSpace._xmouse - MC._parent._xmouse;
		var BaseY:Number = NewParentSpace._ymouse - MC._parent._ymouse;
		Pos.X = BaseX + MC._x;
		Pos.Y = BaseY + MC._y;
		return Pos;
	}
	static private function TranslatePos(MC:MovieClip, X:Number, Y:Number, NewParentSpace:MovieClip):Object
	{
		var Pos:Object = new Object();
		var BaseX:Number = NewParentSpace._xmouse - MC._xmouse;
		var BaseY:Number = NewParentSpace._ymouse - MC._ymouse;
		Pos.X = BaseX + X;
		Pos.Y = BaseY + Y;
		return Pos;
	}
	
}