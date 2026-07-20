#Requires AutoHotkey v2.0
; ============================================================
; ImageSR-Toolkit GUI Wrapper
; ============================================================

#SingleInstance Force
#NoTrayIcon

; ---------- 全局变量 ----------
global MainGui

global EXE_PATH := A_ScriptDir "\ImageSR.exe"
global MODELS := Map(
    "realesrgan",           {scale: [4],       denoise: [], syncgap: [], desc: "realesrgan（真实照片）"},
    "realesrnet",           {scale: [4],       denoise: [], syncgap: [], desc: "realesrnet（真实照片，更自然）"},
    "realesrgan-anime",     {scale: [4],       denoise: [], syncgap: [], desc: "realesrgan-anime（动漫插画）"},
    "realesrgan-animevideo",{scale: [2,3,4],   denoise: [], syncgap: [], desc: "realesrgan-animevideo（免显卡，动漫/视频）"},
    "DF2K",                 {scale: [4],       denoise: [], syncgap: [], desc: "DF2K（Real-ESR 一代）"},
    "DF2K-JPEG",            {scale: [4],       denoise: [], syncgap: [], desc: "DF2K-JPEG（压缩图专用）"},
    "realcugan",            {scale: [1,2,3,4], denoise: [-1,0,1,2,3], syncgap: [0,1,2,3], desc: "realcugan（线条纹理保留好）"},
    "waifu2x-anime",        {scale: [1,2,4,8,16,32], denoise: [-1,0,1,2,3], syncgap: [], desc: "waifu2x-anime（动漫风格）"},
    "waifu2x-photo",        {scale: [1,2,4,8,16,32], denoise: [-1,0,1,2,3], syncgap: [], desc: "waifu2x-photo（真实照片）"}
)

; ---------- 主窗口 ----------
MainGui := Gui("+Resize +MinSize700x520", "ImageSR-Toolkit 图形化界面")
MainGui.SetFont("s10", "Microsoft YaHei UI")
MainGui.BackColor := "0xF5F5F5"
MainGui.OnEvent("Size", GuiResize)
MainGui.OnEvent("Close", (*) => ExitApp())

; --- 标题 ---
MainGui.AddText("x20 y12 w660 h30 Center c0x2D2D2D +0x200", "🖼️ ImageSR-Toolkit 图片超分辨率工具")
MainGui.SetFont("s9")
MainGui.AddText("x20 y40 w660 h20 Center c0x666666", "为命令行工具 ImageSR.exe 提供图形化操作界面")

; --- 输入路径 ---
MainGui.AddGroupBox("x20 y70 w660 h70", "📂 输入路径（支持文件或文件夹）")
InputEdit := MainGui.AddEdit("x35 y95 w520 h25 vInputPath")
InputEdit.OnEvent("Change", InputChanged)
MainGui.AddButton("x565 y95 w100 h25", "浏览...").OnEvent("Click", BrowseInput)

; --- 输出路径 ---
MainGui.AddGroupBox("x20 y150 w660 h70", "💾 输出路径（留空则输出到输入目录）")
OutputEdit := MainGui.AddEdit("x35 y175 w520 h25 vOutputPath")
MainGui.AddButton("x565 y175 w100 h25", "浏览...").OnEvent("Click", BrowseOutput)

; --- 模型与参数 ---
MainGui.AddGroupBox("x20 y230 w360 h200", "⚙️ 模型与参数")
MainGui.AddText("x35 y255 w60 h20 +0x200", "模型:")
ModelDDL := MainGui.AddDropDownList("x95 y255 w270 vModelDDL Choose1", GetModelNames())
ModelDDL.OnEvent("Change", ModelChanged)

MainGui.AddText("x35 y290 w60 h20 +0x200", "放大倍数:")
ScaleDDL := MainGui.AddDropDownList("x95 y290 w190 vScaleDDL", ["4"])

MainGui.AddText("x35 y325 w60 h20 +0x200", "降噪等级:")
DenoiseDDL := MainGui.AddDropDownList("x95 y325 w190 vDenoiseDDL Choose1 +Disabled", ["无需设置"])

MainGui.AddText("x35 y360 w60 h20 +0x200", "分块等级:")
SyncgapDDL := MainGui.AddDropDownList("x95 y360 w190 vSyncgapDDL Choose1 +Disabled", ["无需设置"])

MainGui.AddText("x35 y400 w300 h18 c0x999999", "💡 提示: 真实照片别用 anime 模型")

; --- 选项 ---
MainGui.AddGroupBox("x400 y230 w280 h200", "🔧 处理选项")
MainGui.AddText("x415 y255 w100 h20 +0x200", "文件后缀过滤:")
MainGui.AddEdit("x515 y255 w150 h25 vExtension", "jpg png")
MainGui.AddText("x415 y285 w250 h16 c0x999999", "多个后缀用空格分隔，留空=全部")

MainGui.AddCheckBox("x415 y315 vRecursive", "📁 递归处理子文件夹")
MainGui.AddCheckBox("x415 y345 vForce", "🔄 强制覆盖已有文件")

; --- 操作按钮 ---
MainGui.AddButton("x400 y380 w120 h40 +Default", "▶ 开始处理").OnEvent("Click", StartProcess)
MainGui.AddButton("x535 y380 w120 h40", "📋 复制命令").OnEvent("Click", CopyCommand)

; --- 日志输出 ---
MainGui.AddGroupBox("x20 y440 w660 h140", "📋 运行日志 / 命令预览")
LogEdit := MainGui.AddEdit("x35 y460 w630 h110 vLogEdit ReadOnly +VScroll +HScroll +WantReturn")
LogEdit.SetFont("s9", "Consolas")

; --- 状态栏 ---
StatusBar := MainGui.AddStatusBar()
StatusBar.SetText("就绪 | 请将 ImageSR.exe 与本脚本放在同一目录")

; 显示窗口后再初始化
MainGui.Show("w700 h600")

; 启用拖拽
DllCall("Shell32\DragAcceptFiles", "Ptr", MainGui.Hwnd, "Int", 1)
DllCall("Shell32\DragAcceptFiles", "Ptr", InputEdit.Hwnd, "Int", 1)
DllCall("Shell32\DragAcceptFiles", "Ptr", OutputEdit.Hwnd, "Int", 1)
OnMessage(0x233, WM_DROPFILES)

ModelChanged(ModelDDL, "")

return

; ============================================================
; 函数定义
; ============================================================

GetModelNames() {
    names := []
    for key, val in MODELS
        names.Push(val.desc)
    return names
}

GetModelKeyByDesc(desc) {
    for key, val in MODELS {
        if (val.desc = desc)
            return key
    }
    return ""
}

BrowseInput(*) {
    global MainGui
    folder := DirSelect(A_ScriptDir, 2, "选择输入文件夹")
    if (folder != "") {
        SetInputPath(folder)
        return
    }
    file := FileSelect(1, A_ScriptDir, "选择图片文件", "图片文件 (*.jpg;*.png;*.bmp;*.webp;*.tiff)")
    if (file != "")
        SetInputPath(file)
}

BrowseOutput(*) {
    global MainGui
    folder := DirSelect(A_ScriptDir, 2, "选择输出文件夹")
    if (folder != "")
        MainGui["OutputPath"].Value := folder
}

SetInputPath(path) {
    global MainGui
    MainGui["InputPath"].Value := path
    if (MainGui["OutputPath"].Value = "")
        MainGui["OutputPath"].Value := path
}

InputChanged(ctrl, *) {
    global MainGui
    if (MainGui["OutputPath"].Value = "" && ctrl.Value != "")
        MainGui["OutputPath"].Value := ctrl.Value
}

IsHwndInGui(hwnd, guiObj) {
    if (hwnd = guiObj.Hwnd)
        return true
    parent := DllCall("GetParent", "Ptr", hwnd, "Ptr")
    return (parent = guiObj.Hwnd)
}

WM_DROPFILES(wParam, lParam, msg, hwnd) {
    global MainGui
    if (!IsHwndInGui(hwnd, MainGui))
        return

    fileCount := DllCall("Shell32\DragQueryFileW", "Ptr", wParam, "UInt", 0xFFFFFFFF, "Ptr", 0, "UInt", 0)
    if (fileCount > 0) {
        buf := Buffer(520)
        DllCall("Shell32\DragQueryFileW", "Ptr", wParam, "UInt", 0, "Ptr", buf.Ptr, "UInt", 260)
        path := StrGet(buf, "UTF-16")
        SetInputPath(path)
    }
    DllCall("Shell32\DragFinish", "Ptr", wParam)
    return 0
}

ModelChanged(ctrl, *) {
    global MainGui
    desc := ctrl.Text
    key := GetModelKeyByDesc(desc)
    if (key = "")
        return
    cfg := MODELS[key]

    ; 放大倍数
    scales := []
    for s in cfg.scale
        scales.Push(String(s))
    MainGui["ScaleDDL"].Delete()
    MainGui["ScaleDDL"].Add(scales)
    MainGui["ScaleDDL"].Choose(1)

    ; 降噪
    if (cfg.denoise.Length > 0) {
        denoises := []
        for d in cfg.denoise
            denoises.Push(String(d))
        MainGui["DenoiseDDL"].Delete()
        MainGui["DenoiseDDL"].Add(denoises)
        MainGui["DenoiseDDL"].Choose(1)
        MainGui["DenoiseDDL"].Enabled := true
    } else {
        MainGui["DenoiseDDL"].Delete()
        MainGui["DenoiseDDL"].Add(["无需设置"])
        MainGui["DenoiseDDL"].Choose(1)
        MainGui["DenoiseDDL"].Enabled := false
    }

    ; 分块
    if (cfg.syncgap.Length > 0) {
        gaps := []
        for g in cfg.syncgap
            gaps.Push(String(g))
        MainGui["SyncgapDDL"].Delete()
        MainGui["SyncgapDDL"].Add(gaps)
        MainGui["SyncgapDDL"].Choose(1)
        MainGui["SyncgapDDL"].Enabled := true
    } else {
        MainGui["SyncgapDDL"].Delete()
        MainGui["SyncgapDDL"].Add(["无需设置"])
        MainGui["SyncgapDDL"].Choose(1)
        MainGui["SyncgapDDL"].Enabled := false
    }

    UpdatePreview()
}

BuildCommand(silent := false) {
    global MainGui, EXE_PATH
    inputPath := MainGui["InputPath"].Value
    outputPath := MainGui["OutputPath"].Value
    desc := MainGui["ModelDDL"].Text
    modelKey := GetModelKeyByDesc(desc)
    scale := MainGui["ScaleDDL"].Text
    denoise := MainGui["DenoiseDDL"].Text
    syncgap := MainGui["SyncgapDDL"].Text
    ext := MainGui["Extension"].Value
    recursive := MainGui["Recursive"].Value
    force := MainGui["Force"].Value

    if (inputPath = "") {
        if (!silent)
            MsgBox("请先选择输入路径！", "提示", "Icon!")
        return ""
    }

    cmd := '"' EXE_PATH '"'
    cmd .= ' -i "' inputPath '"'
    if (outputPath != "")
        cmd .= ' -o "' outputPath '"'
    cmd .= ' -m ' modelKey
    cmd .= ' -s ' scale

    cfg := MODELS[modelKey]
    if (cfg.denoise.Length > 0 && denoise != "无需设置")
        cmd .= ' -d ' denoise
    if (cfg.syncgap.Length > 0 && syncgap != "无需设置")
        cmd .= ' -g ' syncgap

    if (ext != "") {
        exts := StrSplit(ext, A_Space)
        cmd .= ' -e'
        for e in exts
            cmd .= ' ' e
    }
    if (recursive)
        cmd .= ' -r'
    if (force)
        cmd .= ' -f'

    return cmd
}

UpdatePreview() {
    global MainGui
    cmd := BuildCommand(true)
    if (cmd != "")
        MainGui["LogEdit"].Value := "[命令预览]`n" cmd "`n`n等待开始处理..."
}

CopyCommand(*) {
    cmd := BuildCommand()
    if (cmd = "")
        return
    A_Clipboard := cmd
    MsgBox("命令已复制到剪贴板！", "提示", "Iconi")
}

StartProcess(*) {
    global MainGui, EXE_PATH

    cmd := BuildCommand()
    if (cmd = "")
        return

    if (!FileExist(EXE_PATH)) {
        MsgBox("未找到 ImageSR.exe！`n请确保 ImageSR.exe 与本脚本位于同一目录。`n`n当前路径: " EXE_PATH, "错误", "Iconx")
        return
    }

    MainGui["LogEdit"].Value := "[开始执行]`n" cmd "`n`n"
    MainGui["LogEdit"].Value .= "正在处理中，请稍候...`n"

    try {
        shell := ComObject("WScript.Shell")
        exec := shell.Exec(cmd)

        output := ""
        while (!exec.StdOut.AtEndOfStream) {
            line := exec.StdOut.ReadLine()
            output .= line "`n"
            MainGui["LogEdit"].Value := "[正在执行]`n" cmd "`n`n" output
            SendMessage(0x115, 7, 0, MainGui["LogEdit"])
        }

        errOutput := ""
        while (!exec.StdErr.AtEndOfStream) {
            line := exec.StdErr.ReadLine()
            errOutput .= line "`n"
        }

        if (errOutput != "")
            output .= "`n[错误输出]`n" errOutput

        MainGui["LogEdit"].Value := "[执行完成]`n" cmd "`n`n" output
        MainGui["LogEdit"].Value .= "`n✅ 处理完成！"

    } catch Error as e {
        MainGui["LogEdit"].Value := "[执行失败]`n" cmd "`n`n错误: " e.Message "`n`n建议: 请检查 ImageSR.exe 是否可正常运行。"
    }

    SendMessage(0x115, 7, 0, MainGui["LogEdit"])
}

GuiResize(GuiObj, MinMax, Width, Height) {
    if (MinMax = -1)
        return
    GuiObj["InputPath"].Move(,, Width - 160)
    GuiObj["OutputPath"].Move(,, Width - 160)
    GuiObj["LogEdit"].Move(,, Width - 70, Height - 500)
}

#HotIf WinActive("ImageSR-Toolkit")
F5::StartProcess()
^c::CopyCommand()
#HotIf
