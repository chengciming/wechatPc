<?php

// PC端微信聊天图片加密文件存放路径：C:\Users\电脑账号\Documents\WeChat Files\微信号\FileStorage\Image
// 加密文件结尾都是以.dat

// 需要解密的dat文件
$encodePath = 'C:/Users/Lemonice/Desktop/c14d0257b67d5291bef56853ee3e3c6d.dat';
// 解密后保存的文件路径
$decodePath = 'C:/Users/Lemonice/Desktop/decode.jpg';

// 打开文件，文件大的话不建议一次性读取
$encode = file_get_contents($encodePath);
// 找出jpg的或异值
$info = checkType(substr($encode,0,2));
if (is_null($info)) {
    echo '检查不出是什么图片。' . "\n";
    exit;
}
// 开始解密
$decode = '';
for ($i = 0; $i < strlen($encode); $i++) {
    $decode .= $encode[$i] ^ chr($info['password']);
}
// 保存解密后的图片
file_put_contents($decodePath, $decode);
echo '这是一张' . $info['ext'] . '图片' . "\n";
echo '图片保存在：' . $decodePath . "\n";
exit;
// 检查图片格式，并且返回解密密钥
function checkType($twoByte)
{
    // 图片文件头
    $header = [
        'jpg' => [0xFF, 0xD8],
        'png' => [0x89, 0x50],
        'gif' => [0x47, 0x49],
    ];
    foreach ($header as $ext => $hex) {
        $strInfo = @unpack("C2chars", $twoByte);
        $password = $strInfo['chars1'] ^ $hex[0];
        $charCheck1 = $strInfo['chars1'] ^ $password;
        $charCheck2 = $strInfo['chars2'] ^ $password;
        if ($charCheck1 == $hex[0] && $charCheck2 == $hex[1]) {
            return ['ext' => $ext, 'password' => $password];
        }
    }
    return null;
}