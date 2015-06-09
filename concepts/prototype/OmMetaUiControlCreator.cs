﻿using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace OmniPrototype
{
    public class OmMetaUiControlCreator
    {
        public delegate FrameworkElement ComponentPlaceholderRequestedHandler(WrapPanel thePanel, ref int thePlaceholderIndex, string thePlaceholderName);

        public OmMetaUiControlCreator (
            ComponentPlaceholderRequestedHandler theExpressionPlaceholderRequested)
        {
            mExpressionPlaceholderRequested = theExpressionPlaceholderRequested;
        }

        public static Brush ColorFromText (string theText)
        {
            // When the text only consists of some kind of brackets and whitespace:
            if (theText.Trim("{}()[] \t\r\n".ToCharArray()).Length == 0)
            {
                return Brushes.Red;
            }
            else
            {
                return Brushes.Blue;
            }
        }

        public static int StringIndexOfNotAny (string theText, string theAny, int theStartIndex)
        {
            for (int i = theStartIndex; i < theText.Length; ++i)
            {
                if (theAny.IndexOf(theText[i]) < 0)
                {
                    return i;
                }
            }
            return -1;
        }

        public static IEnumerable <string> SplitAtBrackets (string theText)
        {
            // variable
            // (variable
            // (
            // = (<input>)
            /*
            foreach (var c in theText)
            {
                yield return c.ToString ();
            }
            */
            const string match = "{}[]() \t\r\n";
            int startIdx = 0;
            int endIdx = 0;
            do
            {
                endIdx = StringIndexOfNotAny(theText, match, startIdx);
                if (endIdx == startIdx)
                {
                    endIdx = theText.IndexOfAny(match.ToCharArray(), startIdx);
                }
                if (endIdx == -1)
                {
                    endIdx = theText.Length;
                }
                yield return theText.Substring(startIdx, endIdx - startIdx);
                startIdx = endIdx;
            } while (endIdx < theText.Length);
        }

        public FrameworkElement CreateControlsFromTemplate(OmContext theContext, StackPanel theLinesPanel, WrapPanel thePanel, ref int thePosition, string theTemplate)
        {
            string[] lines = theTemplate.Split(new string[] { "\r\n" }, System.StringSplitOptions.None);
            foreach (var line in lines)
            {
                var lineRest = line;
                while (lineRest.Length > 0)
                {
                    int freeInputIndex = lineRest.IndexOf("[");
                    int expressionTypeIndex = lineRest.IndexOf("<");
                    if (freeInputIndex >= 0 && (freeInputIndex < expressionTypeIndex || expressionTypeIndex < 0))
                    {
                        // First the free input placeholder
                        string staticText = lineRest.Substring(0, freeInputIndex);
                        if (staticText.Length > 0)
                        {
                            //mGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1.0, GridUnitType.Auto) });
                            foreach (var text in SplitAtBrackets(staticText))
                            {
                                var tb = new TextBlock()
                                {
                                    Text = text,
                                    VerticalAlignment = VerticalAlignment.Center,
                                    Foreground = ColorFromText(text),
                                    Margin = new Thickness (0)
                                };
                                thePanel.Children.Insert (thePosition++, tb);
                            }
                        }
                        lineRest = lineRest.Substring(freeInputIndex + 1);
                        int freeInputIndexEnd = lineRest.IndexOf("]");
                        if (freeInputIndexEnd < 0)
                        {
                            throw new Exception("Unterminated [");
                        }
                        string freeInputName = lineRest.Substring(0, freeInputIndexEnd);
                        thePanel.Children.Insert(thePosition++, mExpressionPlaceholderRequested(thePanel, ref thePosition, freeInputName));
                        lineRest = lineRest.Substring(freeInputIndexEnd + 1);
                    }
                    else if (expressionTypeIndex >= 0)
                    {
                        // First the free expression placeholder
                        string staticText = lineRest.Substring(0, expressionTypeIndex);
                        lineRest = lineRest.Substring(expressionTypeIndex + 1);
                        int expressionTypeIndexEnd = lineRest.IndexOf(">");
                        if (expressionTypeIndexEnd < 0)
                        {
                            throw new Exception("Unterminated <");
                        }
                        if (staticText.Length > 0)
                        {
                            foreach (var text in SplitAtBrackets(staticText))
                            {
                                var tb = new TextBlock()
                                {
                                    Text = text,
                                    VerticalAlignment = VerticalAlignment.Center,
                                    Foreground = ColorFromText(text),
                                    Margin = new Thickness(0)
                                };
                                thePanel.Children.Insert(thePosition++, tb);
                            }
                        }
                        var componentName = lineRest.Substring(0, expressionTypeIndexEnd);
                        var newChild = mExpressionPlaceholderRequested(thePanel, ref thePosition, componentName);
                        if (newChild != null)
                        {
                            thePanel.Children.Insert(thePosition++, newChild);
                        }
                        lineRest = lineRest.Substring(expressionTypeIndexEnd + 1);
                    }
                    else
                    {
                        // No placeholders any more
                        foreach (var text in SplitAtBrackets(lineRest))
                        {
                            var tb = new TextBlock()
                            {
                                Text = text,
                                VerticalAlignment = VerticalAlignment.Center,
                                Foreground = ColorFromText(text),
                                Margin = new Thickness(0)
                            };
                            thePanel.Children.Insert(thePosition++, tb);
                        }
                        lineRest = string.Empty;
                    }
                }
            }
            return null;
        }

        private ComponentPlaceholderRequestedHandler mExpressionPlaceholderRequested;
    }
}