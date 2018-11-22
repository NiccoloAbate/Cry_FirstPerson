/**
 * ...
 * @author Niccolo Abate
 */
class DialogueNodeEditor
{
	var Index:Number;
	
	var Background:MovieClip;
	var Top:MovieClip;
	
	var NewDialogue_Button:MyButton;
	
	var dialogue:Dialogue;
	
	public function DialogueNodeEditor(index:Number)
	{
		Index = index;
		
		Background = _root.attachMovie(DialogueNodeEditorInfo.BACKGROUND_FILE, "NodeEditor_" + Index + "_Background", _root.getNextHighestDepth());
		
		Top = Background.attachMovie(DialogueNodeEditorInfo.TOP_FILE, "NodeEditor_" + Index + "_Top", Background.getNextHighestDepth());
		
		NewDialogue_Button = new MyButton("NodeEditor_" + Index + "_NewDialogue_Button", 0, 0, Top);
		NewDialogue_Button.Initialize_FileNames(DialogueNodeEditorInfo.NEWDIALOGUEBUTTON_BACKGROUND_FILE);
		
		InitializeCallbacks();
		
		// Tests
		//NewDialogue("TestDialogue");
	}
	
	private function MouseHitTest(MC:MovieClip):Boolean
	{
		return MC.hitTest(_root._xmouse, _root._ymouse)
	}
	
	// Callbacks
	private function OnRightClick():Void
	{
		if (Main.HasFocus())
			return;
		if (MouseHitTest(Top))
		{
			
		}
		else
		{
			var Dropdown:DropdownMenu = new DropdownMenu("Menu", Background._xmouse, Background._ymouse, Background);
			var Dropdown1Elements:Array = new Array();
			Dropdown1Elements.push(DropdownMenu.MakeElementInfo("Dialogue Node", null));
			Dropdown1Elements.push(DropdownMenu.MakeElementInfo("Start Node", null));
			Dropdown1Elements.push(DropdownMenu.MakeElementInfo("End Node", null));
			Dropdown.AddElement_Dropdown("New Node", Dropdown1Elements);
			Dropdown.AddElement("Save", null);
		}
	}
	private function OnPress():Void
	{
		if (Main.HasFocus())
			return;
		if (MouseHitTest(Top))
		{
			Background.startDrag(false, 0, 0, 1920 - Background._width, 1080 - Background._height);
		}
	}
	private function OnRelease():Void
	{
		if (Main.HasFocus())
			return;
		Background.stopDrag();
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
			{
				Zoom(Delta * -.05);
			}
		}
	}
	private function InitializeCallbacks():Void
	{
		var THIS:DialogueNodeEditor = this;
		
		Background.onPress = function() { THIS.OnPress(); }
		Background.onRelease = function() { THIS.OnRelease(); }
		Background.onReleaseOutside = function() { THIS.OnReleaseOutside(); }
		Background.onMouseWheel = function(delta:Number) { THIS.OnMouseWheel(delta); }
		Mouse.addListener(Background);
		
		var Menu:ContextMenu = new ContextMenu(function() { THIS.OnRightClick(); });
		Menu.hideBuiltInItems();
		Background.menu = Menu;
		
		NewDialogue_Button.OnPress = function()
		{
			var Dropdown:DropdownMenu = new DropdownMenu("TestDropdown", 0, THIS.NewDialogue_Button.Background_MC._height, THIS.Top);
			Dropdown.AddElement("Text 1", function(){ trace(52); });
			Dropdown.AddElement("Text 2", function(){ trace(53); });
			var Dropdown1Elements:Array = new Array();
			Dropdown1Elements.push(DropdownMenu.MakeElementInfo("D 1", function(){ trace(77); }));
			Dropdown1Elements.push(DropdownMenu.MakeElementInfo("D 2", function(){ trace(78); }));
			var Dropdown1_1Elements:Array = new Array();
			Dropdown1_1Elements.push(DropdownMenu.MakeElementInfo("F 1", function(){ trace(79); }));
			Dropdown1_1Elements.push(DropdownMenu.MakeElementInfo("F 2", function(){ trace(80); }));
			Dropdown1Elements.push(DropdownMenu.MakeElement_DropdownInfo("Dropdown 2", Dropdown1_1Elements));
			//Dropdown.AddElement(Dropdown1Elements[0].Text, Dropdown1Elements[0].Callback);
			Dropdown.AddElement_Dropdown("Dropdown 1", Dropdown1Elements);
			var Dropdown2Elements:Array = new Array();
			Dropdown2Elements.push(DropdownMenu.MakeElementInfo("H 1", null));
			Dropdown2Elements.push(DropdownMenu.MakeElementInfo("H 2", null));
			Dropdown.AddElement_Dropdown("Dropdown 2", Dropdown2Elements);
			THIS.NewDialogue("TestDialogue");
		}
	}
	//~Callbacks
	
	private function Zoom(Amount:Number):Void
	{
		Amount += 1;
		if (Amount > 1  && (Background._width * Amount) >= (1920 - DialogueNodeEditorInfo.ZOOMOFFSET))
		{
			return;
		}
		if (Amount < 1 && (Background._width * Amount) <= DialogueNodeEditorInfo.MINIMUMWIDTH)
			return;
		
		Background._xscale *= Amount;
		Background._yscale *= Amount;
	}
	
	// Dialogue
	public function NewDialogue(Name:String):Dialogue
	{
		if (dialogue)
		{
			dialogue.Save();
			dialogue.Destroy();
			delete dialogue;
		}
		dialogue = new Dialogue;
		dialogue.Name = Name;
		return dialogue;
	}
	public function SaveDialogue():Void
	{
		
	}
	//~Dialouge
	
}