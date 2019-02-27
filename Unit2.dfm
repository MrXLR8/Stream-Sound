object Form2: TForm2
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = #1060#1086#1088#1084#1072' '#1056#1077#1076#1072#1082#1090#1080#1088#1086#1074#1072#1085#1080#1103'/'#1044#1086#1073#1072#1074#1083#1077#1085#1080#1103
  ClientHeight = 169
  ClientWidth = 272
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  GlassFrame.Enabled = True
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object ConfirmButton: TBitBtn
    Left = 175
    Top = 20
    Width = 89
    Height = 49
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    Kind = bkRetry
    NumGlyphs = 2
    TabOrder = 0
    OnClick = ConfirmButtonClick
  end
  object DeleteButton: TBitBtn
    Left = 175
    Top = 98
    Width = 88
    Height = 54
    Caption = #1054#1090#1084#1077#1085#1080#1090#1100
    Kind = bkAbort
    NumGlyphs = 2
    TabOrder = 1
    OnClick = DeleteButtonClick
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 161
    Height = 152
    TabOrder = 2
    object URLText: TLabeledEdit
      Left = 16
      Top = 90
      Width = 121
      Height = 21
      EditLabel.Width = 61
      EditLabel.Height = 13
      EditLabel.Caption = #1040#1076#1088#1077#1089' (URL)'
      TabOrder = 0
    end
    object TitleEdit: TLabeledEdit
      Left = 16
      Top = 40
      Width = 121
      Height = 21
      EditLabel.Width = 50
      EditLabel.Height = 13
      EditLabel.Caption = #1048#1084#1103' (Title)'
      TabOrder = 1
    end
  end
end
