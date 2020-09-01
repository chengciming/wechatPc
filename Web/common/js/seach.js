
// JavaScript Document
//鎼滅储妗�
$(function (){
	$(".bcen_seach_font").click(function (){
		$(this).css("display","none");
		$(this).next().css("display","block");	
		$(this).next().focus();
		$(".bcen_seach").css("background-color","#f5f2f1");
	})
	$(".bcen_seach").find(":text").blur(function (){
		$(".bcen_seach").css("background-color","#e5e3e2");
		var texts = $(this).val();
		if(texts == ""){
			$(this).css("display","none");
			$(this).prev().css("display","block");	
		}	
	})
	$(".bcen_seach").find(":text").keyup(function (){
		var seachs = $(this).val();
		if(seachs != ""){
			$(".bcen_seach_close").css("display","block");
			$(".bcen_seach_but").css("display","none");
			$(".bcen_jilu").eq(0).css("display","none");
			$(".bcen_jilu").eq(1).css("display","block");	
		}else{
			$(".bcen_seach_close").css("display","none");
			$(".bcen_seach_but").css("display","block");
			$(".bcen_jilu").eq(0).css("display","block");
			$(".bcen_jilu").eq(1).css("display","none");	
		}
	})
	$("body").on("click",".bcen_seach_close",function(){
		$(".bcen_seach").find(":text").val("");
		$(".bcen_seach").find(":text").css("display","none");
		$(".bcen_seach_font").css("display","block");
		$(".bcen_seach").css("background-color","#e5e3e2");
		$(".bcen_seach_close").css("display","none");
		$(".bcen_seach_but").css("display","block");
	})
	
	
	//娣诲姞濂藉弸鎼滅储
	$(".ba_left_seach").find(":text").keyup(function (){
		var seachs = $(this).val();
		if(seachs != ""){
			$(".ba_lefts_close").css("display","block");
			$(".ba_lefts_seach").css("display","none");	
		}else{
			$(".ba_lefts_close").css("display","none");
			$(".ba_lefts_seach").css("display","block");	
		}
	})
	$("body").on("click",".ba_lefts_close",function(){
		$(".ba_left_seach").find(":text").val("");
		$(".ba_lefts_close").css("display","none");
		$(".ba_lefts_seach").css("display","block");
	})
	
})