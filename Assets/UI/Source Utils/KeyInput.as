/**
 * ...
 * @author Niccolo Abate
 */
class KeyInput
{
	static private var Listener:Object = {};
	static private var DownMap:Array = [];
	
	static public function Initialize()
	{
		Listener.onKeyDown = function():Void
		{
			if (!KeyInput.DownMap[Key.getCode()])
			{
				KeyInput.DownMap[Key.getCode()] = true;
			}
		};
		Listener.onKeyUp = function():Void
		{
			KeyInput.DownMap[Key.getCode()] = false;
		};
		Key.addListener(Listener);
	}
	
	static public function IsDown(KeyCode:Number):Boolean
	{
		return DownMap[KeyCode];
	}
	
}