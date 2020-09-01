
// JavaScript Document
//涓棿婊氬姩鏉�
function scrolls(){
	var boxs = $(".bcen_box").height();
	var juli = $(".bcen_jilu").height();
	if(juli > boxs){
		//灞忓箷楂樺害梅璁板綍楂樺害
		var boxs_bi = boxs/juli;
		//婊氬姩鏉￠珮搴�
		var scrolls = boxs_bi*boxs;
		scrolls = Math.trunc(scrolls); 
		$(".bcen_scroll").height(scrolls);
	}else{
		$(".bcen_scroll").css("display","none");	
	}
};
$(function (){
	$(".bcen_box").mouseenter(function (){
		var boxs = $(".bcen_box").height();
		var juli = $(".bcen_jilu").height();
		if(juli > boxs){
			$(".bcen_scroll").css("display","block");
		}
	})
	
	$(".bcen_box").mouseleave(function (){
		$(".bcen_scroll").css("display","none");
	})
	
	$(".bcen_box").scroll(function (){
		var boxs = $(".bcen_box").height();
		$(".bcen_scroll").css("display","block");
		//婊氬姩鏉￠珮搴�
		var tiao = $(".bcen_scroll").height();
		if(tiao > 0){
			//婊氬姩鏉″疄闄呮粴鍔ㄨ窛绂�
			var scrs = $(this).scrollTop();
			var tops = scrs + 62;
			var gao = boxs - tiao + 62;
			if(tops > gao){
				tops = gao;	
			}
			$(".bcen_scroll").css("top",tops + "px");
		}
	})
});

//棣栭〉瀵硅瘽婊氬姩鏉�
function chatscr(){
	var bboxs = $(".brce_block_box").height();
	var bjl = $(".brce_block_box").children("div").height();
	if(bjl > bboxs){
		//灞忓箷楂樺害梅璁板綍楂樺害
		var bboxs_bi = bboxs/bjl;
		//婊氬姩鏉￠珮搴�
		var scrs = bboxs_bi*bboxs;
		scrs = Math.trunc(scrs); 
		$(".bri_center_scroll").height(scrs);
	}else{
		$(".bri_center_scroll").css("display","none");	
	}
};
$(function (){
	$(".brce_block_box").mouseenter(function (){
		var bboxs = $(".brce_block_box").height();
		var bjl = $(".brce_block_box").children("div").height();
		if(bjl > bboxs){
			$(".bri_center_scroll").css("display","block");
		}
	})
	
	$(".brce_block_box").mouseleave(function (){
		$(".bri_center_scroll").css("display","none");
	})
	
	$(".brce_block_box").scroll(function (){
		var bboxs = $(".brce_block_box").height();
		$(".bri_center_scroll").css("display","block");
		//婊氬姩鏉￠珮搴�
		var tiao = $(".bri_center_scroll").height();
		if(tiao > 0){
			//婊氬姩鏉″疄闄呮粴鍔ㄨ窛绂�
			var scrs = $(this).scrollTop();
			var tops = scrs;
			var gao = bboxs - tiao;
			if(tops > gao){
				tops = gao;	
			}
			$(".bri_center_scroll").css("top",tops + "px");
		}
	})
});
//娣诲姞鏈嬪弸宸︽粴鍔ㄦ潯
function friscr(){
	/*var boxs = 430;*/
	var ble = $(".ba_leftf_box").children("div").height();
	if(ble > 430){
		//灞忓箷楂樺害梅璁板綍楂樺害
		var ble_bi = 430/ble;
		//婊氬姩鏉￠珮搴�
		var scs = ble_bi*430;
		scs = Math.trunc(scs); 
		$(".ba_leftf_scroll").height(scs);
	}else{
		$(".ba_leftf_scroll").css("display","none");	
	}
};
$(function (){
	$(".ba_leftf_box").mouseenter(function (){
		var ble = $(".ba_leftf_box").children("div").height();
		if(ble > 430){
			$(".ba_leftf_scroll").css("display","block");
		}
	})
	
	$(".ba_leftf_box").mouseleave(function (){
		$(".ba_leftf_scroll").css("display","none");
	})
	
	$(".ba_leftf_box").scroll(function (){
		$(".ba_leftf_scroll").css("display","block");
		//婊氬姩鏉￠珮搴�
		var gdt = $(".ba_leftf_scroll").height();
		if(gdt > 0){
			//婊氬姩鏉″疄闄呮粴鍔ㄨ窛绂�
			var scrs = $(this).scrollTop();
			var tops = scrs;
			var gao = 430 - gdt;
			if(tops > gao){
				tops = gao;	
			}
			$(".ba_leftf_scroll").css("top",tops + "px");
		}
	})
});
//娣诲姞鏈嬪弸鍙虫粴鍔ㄦ潯
function friscrs(){
	/*var boxs = 380;*/
	var bre = $(".ba_rif_box").children("div").height();
	if(bre > 380){
		//灞忓箷楂樺害梅璁板綍楂樺害
		var bre_bi = 380/bre;
		//婊氬姩鏉￠珮搴�
		var scss = bre_bi*380;
		scss = Math.trunc(scss); 
		$(".ba_rif_scroll").height(scss);
	}else{
		$(".ba_rif_scroll").css("display","none");	
	}
};
$(function (){
	$(".ba_rif_box").mouseenter(function (){
		var bre = $(".ba_rif_box").children("div").height();
		if(bre > 380){
			$(".ba_rif_scroll").css("display","block");
		}
	})
	
	$(".ba_rif_box").mouseleave(function (){
		$(".ba_rif_scroll").css("display","none");
	})
	
	$(".ba_rif_box").scroll(function (){
		$(".ba_rif_scroll").css("display","block");
		//婊氬姩鏉￠珮搴�
		var gdt = $(".ba_rif_scroll").height();
		if(gdt > 0){
			//婊氬姩鏉″疄闄呮粴鍔ㄨ窛绂�
			var scrs = $(this).scrollTop();
			var tops = scrs;
			var gao = 380 - gdt;
			if(tops > gao){
				tops = gao;	
			}
			$(".ba_rif_scroll").css("top",tops + "px");
		}
	})
});
