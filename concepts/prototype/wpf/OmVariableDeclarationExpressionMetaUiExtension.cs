﻿using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace OmniPrototype
{
    public class OmVariableDeclarationExpressionMetaUiExtension : OmMetaUiExtension
    {
        public OmVariableDeclarationExpressionMetaUiExtension() : base ("variable_declaration_expression")
        {
        }

        public override OmEntityExtension CreateExtension ()
        {
            return new OmVariableDeclarationUiExtension();
        }

        /**
        @return First list entry is the in-place list of controls, the rest are new-line controls.
        **/
        public override IEnumerable<List<FrameworkElement>> CreateControls(OmContext theContext, OmStatement theExpression)
        {
            var ext = theExpression.GetExtension(theContext, "omni.ui") as OmVariableDeclarationUiExtension;
            var varDecl = theExpression as OmVariableDeclarationExpression;

            varDecl.NameChanged += (OmEntity theSender) =>
            {
                if (ext.NameInput.Text != varDecl.Name)
                {
                    ext.NameInput.Text = varDecl.Name;
                }
            };
            varDecl.InitializationExpressionChanged += (OmEntity theSender) =>
            {
                ext.InitializationExpressionInput.ReplaceWithExpression(theContext, varDecl.InitializationExpression);
            };

            var creator = new OmMetaUiControlCreator(
                (string thePlaceholderName) =>
                {
                    if (thePlaceholderName == "name")
                    {
                        ext.NameInput = new TextBox()
                        {
                            Text = varDecl.Name,
                            MinWidth = 40,
                            Foreground = Brushes.DarkRed,
                            VerticalAlignment=VerticalAlignment.Center
                        };
                        ext.NameInput.TextChanged += (object sender, TextChangedEventArgs e) =>
                        {
                            varDecl.Name = ext.NameInput.Text;
                        };
                        return MakeSingleControlList (ext.NameInput);
                    }
                    else if (thePlaceholderName == "initexpr")
                    {
                        return ExpressionInputControl.CreateInputOrControls(
                            theContext,
                            varDecl,
                            varDecl.InitializationExpression,
                            OmType.Void,
                            (ExpressionInputControl theInput) =>
                            {
                                ext.InitializationExpressionInput = theInput;
                                ext.InitializationExpressionInput.VerticalAlignment = VerticalAlignment.Center;
                                theInput.ExpressionCreated += (ExpressionInputControl theSender, OmStatement theNewExpression) =>
                                {
                                    varDecl.InitializationExpression = theNewExpression as OmExpression;
                                };
                            });
                    }
                    else
                    {
                        throw new Exception(string.Format("In OmVariableDeclarationExpressionMetaUiExtension: Unknown text placeholder {0}", thePlaceholderName));
                    }

                });
            return creator.CreateControlsFromTemplate(theContext, theExpression, GetTemplate(theContext));
        }

    }
}
