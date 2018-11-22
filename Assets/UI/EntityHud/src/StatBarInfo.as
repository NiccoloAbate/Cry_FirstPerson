/**
 * ...
 * @author Niccolo Abate
 */
class StatBarInfo
{
	static var Y_OFFSET:Number = 0;
	
	static var HEALTHBAR_FILE:String = "lib.HealthBar.png";
	static var HEALTHBAR_BACKGROUND_FILE:String = "lib.HealthBar_Background.png";
	static var STAMINABAR_FILE:String = "lib.StaminaBar.png";
	static var STAMINABAR_BACKGROUND_FILE:String = "lib.HealthBar_Background.png";
	static function GetBarFileFromType(Type:String):String
	{
		Type = Type.toLowerCase();
		if (Type == "health")
			return HEALTHBAR_FILE;
		if (Type == "stamina")
			return STAMINABAR_FILE;
		
		return "";
	}
	static function GetBarBacrgroundFileFromType(Type:String):String
	{
		Type = Type.toLowerCase();
		if (Type == "health")
			return HEALTHBAR_BACKGROUND_FILE;
		if (Type == "stamina")
			return STAMINABAR_BACKGROUND_FILE;
		
		return "";
	}
	
}