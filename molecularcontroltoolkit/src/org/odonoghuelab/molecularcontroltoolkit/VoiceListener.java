package org.odonoghuelab.molecularcontroltoolkit;

/**
 * Interface for handling voice events
 * @author KennySabir
 *
 */
public interface VoiceListener {

	/**
	 * Zoom to selection
	 */
	public abstract void zoomToSelection();

	/**
	 * Speech event occurred, currently NOT USED.
	 * @param speechEvent
	 */
	public abstract void triggerSpeech(int speechEvent);

	/**
	 * Reset the view
	 */
	public abstract void reset();

	/**
	 * Search for a term
	 * @param searchTerm
	 */
	public abstract void search(char searchTerm);

	/**
	 * Set the coloring
	 * @param type the colour type
	 */
	public abstract void color(String type);

	/**
	 * Rotate
	 * @param goCrazy true if the spin should be crazy fast.
	 */
	public abstract void spin(boolean goCrazy);

	/**
	 * copy the current selection to the clipboard
	 */
	public abstract void copy();

	/**
	 * paste the current selection from the clipboard
	 */
	public abstract void paste();

	/**
	 * select all residues
	 */
	public abstract void selectAll();

	/**
	 * Select a specific target
	 * @param value
	 */
	public abstract void select(String value);

}
