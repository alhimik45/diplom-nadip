object Form1: TForm1
  Left = 239
  Top = 204
  Width = 983
  Height = 430
  Caption = 'RPN_REPL'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 120
  TextHeight = 16
  object Label1: TLabel
    Left = 537
    Top = 9
    Width = 34
    Height = 16
    Caption = 'Stack'
  end
  object Label2: TLabel
    Left = 688
    Top = 9
    Width = 40
    Height = 16
    Caption = 'Hand1'
  end
  object Label3: TLabel
    Left = 848
    Top = 9
    Width = 40
    Height = 16
    Caption = 'Hand2'
  end
  object Memo1: TMemo
    Left = 0
    Top = 0
    Width = 481
    Height = 337
    TabOrder = 0
  end
  object Edit1: TEdit
    Left = 0
    Top = 336
    Width = 481
    Height = 24
    TabOrder = 1
    OnKeyPress = Edit1KeyPress
  end
  object ListBox1: TListBox
    Left = 489
    Top = 41
    Width = 144
    Height = 288
    ItemHeight = 16
    TabOrder = 2
  end
  object ListBox2: TListBox
    Left = 640
    Top = 41
    Width = 145
    Height = 288
    ItemHeight = 16
    TabOrder = 3
  end
  object ListBox3: TListBox
    Left = 800
    Top = 41
    Width = 145
    Height = 288
    ItemHeight = 16
    TabOrder = 4
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 366
    Width = 975
    Height = 19
    Panels = <>
    SimplePanel = False
  end
end
